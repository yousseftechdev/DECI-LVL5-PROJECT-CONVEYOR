#include <avr/interrupt.h>
#include "GPIO_interface.h"
#include "EXTI_interface.h"
#include "ADC_interface.h"
#include "PWM_interface.h"
#include "UART_interface.h"
#include "MOTOR_interface.h"
#include "LED_CFG.h"

#define LOG(str) UART_voidTransmitString((const u8*) (str))

/* ============================================================================= */
/* GLOBAL FLAGS                                                                  */
/* ============================================================================= */
volatile u8 Flag_StartPressed    = 0;     /* Set when the start button is pressed, cleared after startup action */
volatile u8 Flag_StopPressed     = 0;     /* Set when the stop button is pressed, cleared after shutdown action */
volatile u8 Flag_EmergencyStop   = 0;     /* Set in the INT0 ISR and never cleared until reset */
volatile u8 Flag_ConveyorRunning = 0;     /* Set when the conveyor starts, cleared when it stops */
volatile u8 Flag_ObjectDetected  = 0;     /* Set in the INT1 ISR when an object is sensed */
volatile u8 Flag_SpeedChanged    = 0;     /* Set when the detected speed level changes */
volatile u8 Flag_LoggedEmergency = 0;     /* Set once the emergency log statement has been printed */

static volatile u8 g_u8ObjectISRGuard = 0;  /* Prevents repeated object ISR events from re-triggering the same detection */
static volatile u8 g_u8EmergencyISRGuard = 0;/* Prevents repeated emergency ISR events from re-triggering the same stop */
static u8 g_u8LastSpeedLevel = 0;           /* Tracks the last speed band to detect changes */
static u16 g_u16ObjectCount = 0;            /* Counts valid object detections while the conveyor is running */

static void APP_voidSendCountMessage(u16 Copy_u16Count)
{
    u8 Local_au8Digits[5];
    u8 Local_u8Count = 0;
    u8 Local_u8Index;

    LOG("Object Detected - Count: ");
    if (Copy_u16Count == 0)
    {
        LOG("0\r\n");
        return;
    }

    while (Copy_u16Count > 0)
    {
        Local_au8Digits[Local_u8Count] = (u8)('0' + (Copy_u16Count % 10));
        Copy_u16Count /= 10;
        Local_u8Count++;
    }

    for (Local_u8Index = 0; Local_u8Index < Local_u8Count; Local_u8Index++)
    {
        UART_voidTransmitByte(Local_au8Digits[Local_u8Count - 1 - Local_u8Index]);
    }

    LOG("\r\n");
}

static void APP_voidEmergencyISRCallback(void)
{
    if (g_u8EmergencyISRGuard == 0)
    {
        g_u8EmergencyISRGuard = 1;
        Flag_EmergencyStop = 1;
        MOTOR_voidSetSpeed(0);
        Flag_ConveyorRunning = 0;
    }
}

static void APP_voidObjectISRCallback(void)
{
    if (g_u8ObjectISRGuard == 0)
    {
        g_u8ObjectISRGuard = 1;
        Flag_ObjectDetected = 1;
    }
}

static void APP_voidInitHardware(void)
{
    /* Configure the status LEDs as outputs */
    GPIO_voidSetPinDirection(LED_PORT, LED_RUNNING, GPIO_OUTPUT);
    GPIO_voidSetPinDirection(LED_PORT, LED_IDLE, GPIO_OUTPUT);
    GPIO_voidSetPinDirection(LED_PORT, LED_FAULT, GPIO_OUTPUT);

    GPIO_voidWritePinValue(LED_PORT, LED_RUNNING, GPIO_LOW);
    GPIO_voidWritePinValue(LED_PORT, LED_IDLE, GPIO_LOW);
    GPIO_voidWritePinValue(LED_PORT, LED_FAULT, GPIO_LOW);

    /* Configure the start and stop buttons as inputs with pull-up resistors */
    GPIO_voidSetPinDirection(GPIO_PORTB, GPIO_PIN0, GPIO_INPUT);
    GPIO_voidWritePinValue(GPIO_PORTB, GPIO_PIN0, GPIO_HIGH);

    GPIO_voidSetPinDirection(GPIO_PORTB, GPIO_PIN1, GPIO_INPUT);
    GPIO_voidWritePinValue(GPIO_PORTB, GPIO_PIN1, GPIO_HIGH);

    /* Configure EXTI pins for emergency and object sensing */
    GPIO_voidSetPinDirection(GPIO_PORTD, GPIO_PIN2, GPIO_INPUT);
    GPIO_voidWritePinValue(GPIO_PORTD, GPIO_PIN2, GPIO_HIGH);

    GPIO_voidSetPinDirection(GPIO_PORTD, GPIO_PIN3, GPIO_INPUT);
    GPIO_voidWritePinValue(GPIO_PORTD, GPIO_PIN3, GPIO_HIGH);

    /* Initialize the peripheral drivers */
    MOTOR_voidInit();
    MOTOR_voidSetDirectionCW();
    MOTOR_voidSetSpeed(0);

    ADC_voidInit(ADC_REF_AVCC, ADC_PRESCALER_128);
    PWM_voidInit(PWM_CHANNEL_0, PWM_MODE_FAST, PWM_NON_INVERTING, PWM_PRESCALER_64);
    UART_voidInit(UART_BAUD_9600, UART_PARITY_NONE, UART_STOP_ONE);

    EXTI_voidInit(EXTI_INT0, EXTI_FALLING_EDGE);
    EXTI_voidSetCallBack(EXTI_INT0, APP_voidEmergencyISRCallback);

    EXTI_voidInit(EXTI_INT1, EXTI_FALLING_EDGE);
    EXTI_voidSetCallBack(EXTI_INT1, APP_voidObjectISRCallback);

    sei();
}

int main(void)
{
    u16 Local_u16ADCValue = 0;
    u8 Local_u8SpeedLevel = 0;
    u8 Local_u8DutyCycle = 0;

    APP_voidInitHardware();
    LOG("System Ready\r\n");

    while (1)
    {
        if (Flag_LoggedEmergency) continue;
        if (Flag_EmergencyStop == 1)
        {
            GPIO_voidWritePinValue(LED_PORT, LED_FAULT, GPIO_HIGH);
            GPIO_voidWritePinValue(LED_PORT, LED_RUNNING, GPIO_LOW);
            GPIO_voidWritePinValue(LED_PORT, LED_IDLE, GPIO_LOW);

            if (Flag_ConveyorRunning == 1)
            {
                MOTOR_voidSetSpeed(0);
                Flag_ConveyorRunning = 0;
            }

            LOG("Emergency Stop Activated\r\n");
            Flag_LoggedEmergency = 1;
            continue;
        }

        if (GPIO_u8ReadPinValue(GPIO_PORTB, GPIO_PIN0) == GPIO_LOW)
        {
            Flag_StartPressed = 1;
        }
        else
        {
            Flag_StartPressed = 0;
        }

        if (GPIO_u8ReadPinValue(GPIO_PORTB, GPIO_PIN1) == GPIO_LOW)
        {
            Flag_StopPressed = 1;
        }
        else
        {
            Flag_StopPressed = 0;
        }

        if (Flag_StartPressed == 1 && Flag_ConveyorRunning == 0)
        {
            MOTOR_voidSetSpeed(85);
            Flag_ConveyorRunning = 1;
            Flag_StartPressed = 0;
            GPIO_voidWritePinValue(LED_PORT, LED_RUNNING, GPIO_HIGH);
            GPIO_voidWritePinValue(LED_PORT, LED_IDLE, GPIO_LOW);
            LOG("Conveyor Running\r\n");
        }

        if (Flag_StopPressed == 1 && Flag_ConveyorRunning == 1)
        {
            MOTOR_voidSetSpeed(0);
            Flag_ConveyorRunning = 0;
            Flag_StopPressed = 0;
            GPIO_voidWritePinValue(LED_PORT, LED_RUNNING, GPIO_LOW);
            GPIO_voidWritePinValue(LED_PORT, LED_IDLE, GPIO_HIGH);
            LOG("Conveyor Stopped\r\n");
        }

        if (Flag_ConveyorRunning == 1)
        {
            Local_u16ADCValue = ADC_u16Read(ADC_CHANNEL_0);

            if (Local_u16ADCValue <= 340)
            {
                Local_u8SpeedLevel = 1;
                Local_u8DutyCycle = 85;
            }
            else if (Local_u16ADCValue <= 681)
            {
                Local_u8SpeedLevel = 2;
                Local_u8DutyCycle = 170;
            }
            else
            {
                Local_u8SpeedLevel = 3;
                Local_u8DutyCycle = 255;
            }

            if (Local_u8SpeedLevel != g_u8LastSpeedLevel)
            {
                MOTOR_voidSetSpeed(Local_u8DutyCycle);
                g_u8LastSpeedLevel = Local_u8SpeedLevel;
                Flag_SpeedChanged = 1;
            }

            if (Flag_SpeedChanged == 1)
            {
                if (Local_u8SpeedLevel == 1)
                {
                    LOG("Speed Level: Low\r\n");
                }
                else if (Local_u8SpeedLevel == 2)
                {
                    LOG("Speed Level: Medium\r\n");
                }
                else
                {
                    LOG("Speed Level: High\r\n");
                }

                Flag_SpeedChanged = 0;
            }
        }
        else
        {
            Flag_ObjectDetected = 0;
            g_u8ObjectISRGuard = 0;
        }

        if (Flag_ObjectDetected == 1 && Flag_ConveyorRunning == 1)
        {
            g_u16ObjectCount++;
            Flag_ObjectDetected = 0;
            g_u8ObjectISRGuard = 0;
            GPIO_voidWritePinValue(LED_PORT, LED_IDLE, GPIO_HIGH);
            APP_voidSendCountMessage(g_u16ObjectCount);
        }
    }
}