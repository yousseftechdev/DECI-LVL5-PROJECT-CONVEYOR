/*********************************************************************************/
/* File:        EXTI_program.c                                                   */
/* Author:      Embedded Systems Level 5                                         */
/* Description: Low-Level Hardware Manipulation Source Code for ATmega32 EXTI.   */
/* Uses standard <avr/io.h> register naming and handles custom ISR logic mappings.*/
/*********************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h> /* Gives compiler structural vector hooks for ISR syntax */
#include "EXTI_interface.h"

/* Null Definition for defensive callback initialization guards */
#define NULL    ((void*)0)

/* Private global array of function pointers storing registered upper-layer callback addresses */
static void (*EXTI_pvCallBackNotification[3])(void) = {NULL, NULL, NULL};

/* ============================================================================= */
/* IMPLEMENTATIONS: PERIPHERAL CONFIGURATION AND CONTROL FUNCTIONS               */
/* ============================================================================= */

void EXTI_voidInit(u8 Copy_u8InterruptLine, u8 Copy_u8SenseControl)
{
    /* Configure the trigger mode condition first */
    EXTI_voidSetSenseControl(Copy_u8InterruptLine, Copy_u8SenseControl);
    
    /* Enable the peripheral interrupt line path */
    EXTI_voidEnableInterrupt(Copy_u8InterruptLine);
}


void EXTI_voidSetSenseControl(u8 Copy_u8InterruptLine, u8 Copy_u8SenseControl)
{
    switch(Copy_u8InterruptLine)
    {
        case EXTI_INT0:
            /* Mask away existing INT0 mode bits (ISC00, ISC01) inside MCU Control Register */
            MCUCR &= ~((1 << ISC00) | (1 << ISC01));
            
            /* Inject new configurations into target register location */
            if (Copy_u8SenseControl == EXTI_LOW_LEVEL)          { /* Bits already cleared to 00 */ }
            else if (Copy_u8SenseControl == EXTI_LOGICAL_CHANGE){ MCUCR |= (1 << ISC00); }
            else if (Copy_u8SenseControl == EXTI_FALLING_EDGE)  { MCUCR |= (1 << ISC01); }
            else if (Copy_u8SenseControl == EXTI_RISING_EDGE)   { MCUCR |= ((1 << ISC00) | (1 << ISC01)); }
            break;

        case EXTI_INT1:
            /* Mask away existing INT1 mode bits (ISC10, ISC11) inside MCU Control Register */
            MCUCR &= ~((1 << ISC10) | (1 << ISC11));
            
            if (Copy_u8SenseControl == EXTI_LOW_LEVEL)          { /* Bits already cleared to 00 */ }
            else if (Copy_u8SenseControl == EXTI_LOGICAL_CHANGE){ MCUCR |= (1 << ISC10); }
            else if (Copy_u8SenseControl == EXTI_FALLING_EDGE)  { MCUCR |= (1 << ISC11); }
            else if (Copy_u8SenseControl == EXTI_RISING_EDGE)   { MCUCR |= ((1 << ISC10) | (1 << ISC11)); }
            break;

        case EXTI_INT2:
            /* INT2 is a unique 1-bit control module located inside MCUCSR register (ISC2) */
            if (Copy_u8SenseControl == EXTI_FALLING_EDGE)
            {
                MCUCSR &= ~(1 << ISC2); /* Clearing ISC2 configures a Falling Edge trigger constraint */
            }
            else if (Copy_u8SenseControl == EXTI_RISING_EDGE)
            {
                MCUCSR |=  (1 << ISC2); /* Setting ISC2 configures a Rising Edge trigger constraint */
            }
            break;
    }
}


void EXTI_voidEnableInterrupt(u8 Copy_u8InterruptLine)
{
    switch(Copy_u8InterruptLine)
    {
        /* General Interrupt Control Register (GICR) acts as our Local PIE Key mapping */
        case EXTI_INT0: GICR |= (1 << INT0); break;
        case EXTI_INT1: GICR |= (1 << INT1); break;
        case EXTI_INT2: GICR |= (1 << INT2); break;
    }
}


void EXTI_voidDisableInterrupt(u8 Copy_u8InterruptLine)
{
    switch(Copy_u8InterruptLine)
    {
        case EXTI_INT0: GICR &= ~(1 << INT0); break;
        case EXTI_INT1: GICR &= ~(1 << INT1); break;
        case EXTI_INT2: GICR &= ~(1 << INT2); break;
    }
}


void EXTI_voidSetCallBack(u8 Copy_u8InterruptLine, void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        /* Register callback reference address internally to target channel location index */
        EXTI_pvCallBackNotification[Copy_u8InterruptLine] = Copy_pvCallBackFunc;
    }
}

/* ============================================================================= */
/* INTERRUPT SERVICE ROUTINES (ISR) - ATMEGA32 VECTOR INTERFACES                 */
/* ============================================================================= */

/**
 * @brief ISR handler for Vector 2: External Hardware Interrupt Request 0 (INT0)
 */
ISR(INT0_vect)
{
    if (EXTI_pvCallBackNotification[EXTI_INT0] != NULL)
    {
        /* Execute the registered upper-layer app code without driver layer mixing */
        EXTI_pvCallBackNotification[EXTI_INT0]();
    }
}

/**
 * @brief ISR handler for Vector 3: External Hardware Interrupt Request 1 (INT1)
 */
ISR(INT1_vect)
{
    if (EXTI_pvCallBackNotification[EXTI_INT1] != NULL)
    {
        EXTI_pvCallBackNotification[EXTI_INT1]();
    }
}

/**
 * @brief ISR handler for Vector 4: External Hardware Interrupt Request 2 (INT2)
 */
ISR(INT2_vect)
{
    if (EXTI_pvCallBackNotification[EXTI_INT2] != NULL)
    {
        EXTI_pvCallBackNotification[EXTI_INT2]();
    }
}

