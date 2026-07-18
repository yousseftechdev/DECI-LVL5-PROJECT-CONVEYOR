/*********************************************************************************/
/* File:        BUTTON_program.c                                                 */
/* Author:      Embedded Systems Level 5                                         */
/* Description: Button Abstraction Mapping Inputs to EXTI Interrupt Flags         */
/*********************************************************************************/

#include "GPIO_interface.h"
#include "EXTI_interface.h"
#include "BUTTON_interface.h"

void BUTTON_voidInitInterruptButton(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8InterruptChannel, u8 Copy_u8PullType)
{
	/* 1. Force the physical microcontroller pin direction to input mode */
	GPIO_voidSetPinDirection(Copy_u8Port, Copy_u8Pin, GPIO_INPUT);
	
	/* 2. Configure bias configurations and determine the interrupt edge trigger */
	if (Copy_u8PullType == BUTTON_PULLUP)
	{
		GPIO_voidWritePinValue(Copy_u8Port, Copy_u8Pin, GPIO_HIGH); /* Engage active internal pull-up */
		EXTI_voidInit(Copy_u8InterruptChannel, EXTI_FALLING_EDGE);  /* Pressing drops line to Ground */
	}
	else
	{
		GPIO_voidWritePinValue(Copy_u8Port, Copy_u8Pin, GPIO_LOW);  /* Enforce external pull-down */
		EXTI_voidInit(Copy_u8InterruptChannel, EXTI_RISING_EDGE);   /* Pressing pulls line to VCC */
	}
}