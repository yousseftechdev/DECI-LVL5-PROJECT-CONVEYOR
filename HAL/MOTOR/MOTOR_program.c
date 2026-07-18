/*********************************************************************************/
/* File:        MOTOR_program.c                                                  */
/* Author:      Embedded Systems Level 5                                         */
/* Description: Core Control Logic abstracting L293D/L298N H-Bridge Signaling     */
/*********************************************************************************/

#include "GPIO_interface.h"
#include "PWM_interface.h"
#include "MOTOR_interface.h"

void MOTOR_voidInit(void)
{
	/* 1. Configure direction control pins as digital outputs */
	GPIO_voidSetPinDirection(GPIO_PORTD, GPIO_PIN4, GPIO_OUTPUT); /* IN1 */
	GPIO_voidSetPinDirection(GPIO_PORTD, GPIO_PIN5, GPIO_OUTPUT); /* IN2 */

	/* 2. Configure Timer0 PWM generation pin PB3 (OC0) as a digital output */
	GPIO_voidSetPinDirection(GPIO_PORTB, GPIO_PIN3, GPIO_OUTPUT); /* ENA */

	/* 3. Start the PWM engine on Channel 0 (Fast PWM, Non-Inverting, Clock Div 64) */
	PWM_voidInit(PWM_CHANNEL_0, PWM_MODE_FAST, PWM_NON_INVERTING, PWM_PRESCALER_64);
}

void MOTOR_voidSetDirectionCW(void)
{
	/* IN1 = HIGH, IN2 = LOW */
	GPIO_voidWritePinValue(GPIO_PORTD, GPIO_PIN4, GPIO_HIGH);
	GPIO_voidWritePinValue(GPIO_PORTD, GPIO_PIN5, GPIO_LOW);
}

void MOTOR_voidSetDirectionCCW(void)
{
	/* IN1 = LOW, IN2 = HIGH */
	GPIO_voidWritePinValue(GPIO_PORTD, GPIO_PIN4, GPIO_LOW);
	GPIO_voidWritePinValue(GPIO_PORTD, GPIO_PIN5, GPIO_HIGH);
}

void MOTOR_voidSetSpeed(u8 Copy_u8SpeedValue)
{
	/* Push speed parameters down to the PWM hardware register layer */
	PWM_voidSetCompareMatchValue(PWM_CHANNEL_0, Copy_u8SpeedValue);
}