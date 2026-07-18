/*********************************************************************************/
/* File:        PWM_program.c                                                    */
/* Author:      Embedded Systems Level 5                                         */
/* Description: Low-Level Hardware Manipulation Source Code for ATmega32 PWM.   */
/*********************************************************************************/

#include <avr/io.h>
#include "PWM_interface.h"

/* Private Prescaler Look-up Mapping Arrays */
static const u8 PWM_u8PrescalerBits_T0_T1[6] = {0, 1, 2, 3, 4, 5};
static const u8 PWM_u8PrescalerBits_T2[6]    = {0, 1, 2, 4, 6, 7};

/* ============================================================================= */
/* IMPLEMENTATIONS: LOW-LEVEL CONFIGURATION ENGINE FUNCTIONS                      */
/* ============================================================================= */

void PWM_voidInit(u8 Copy_u8Channel, u8 Copy_u8PWMMode, u8 Copy_u8Polarity, u8 Copy_u8Prescaler)
{
	switch(Copy_u8Channel)
	{
		case PWM_CHANNEL_0:
		/* Mask away existing waveform generation mode bits and compare match output modes */
		TCCR0 &= ~((1 << WGM00) | (1 << WGM01) | (1 << COM00) | (1 << COM01) | (1 << CS00) | (1 << CS01) | (1 << CS02));
		
		/* Establish the Waveform Generation Mode (WGM) bits for Timer0 */
		if (Copy_u8PWMMode == PWM_MODE_FAST)
		{
			TCCR0 |= ((1 << WGM00) | (1 << WGM01)); /* Fast PWM Mode */
		}
		else if (Copy_u8PWMMode == PWM_MODE_PHASE_CORRECT)
		{
			TCCR0 |= (1 << WGM00);                 /* Phase Correct PWM Mode */
		}

		/* Route the Compare Match Output (COM) bits for pin output behavior */
		if (Copy_u8Polarity == PWM_NON_INVERTING)
		{
			TCCR0 |= (1 << COM01);                 /* Clear OC0 on match, set at TOP */
		}
		else if (Copy_u8Polarity == PWM_INVERTING)
		{
			TCCR0 |= ((1 << COM00) | (1 << COM01)); /* Set OC0 on match, clear at TOP */
		}
		
		/* Apply the hardware clock prescaler */
		TCCR0 |= PWM_u8PrescalerBits_T0_T1[Copy_u8Prescaler];
		break;

		case PWM_CHANNEL_1A:
		/* Clear configuration layouts across the split TCCR1A and TCCR1B registers */
		TCCR1A &= ~((1 << WGM10) | (1 << WGM11) | (1 << COM1A0) | (1 << COM1A1));
		TCCR1B &= ~((1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS11) | (1 << CS12));
		
		/* Configure Mode: Targeting 8-Bit Fixed Resolution PWM Configurations */
		if (Copy_u8PWMMode == PWM_MODE_FAST)
		{
			TCCR1A |= (1 << WGM10);
			TCCR1B |= (1 << WGM12); /* 8-Bit Fast PWM mode selection */
		}
		else if (Copy_u8PWMMode == PWM_MODE_PHASE_CORRECT)
		{
			TCCR1A |= (1 << WGM10); /* 8-Bit Phase Correct PWM mode selection */
		}

		/* Set signal output polarity on physical pin OC1A */
		if (Copy_u8Polarity == PWM_NON_INVERTING)
		{
			TCCR1A |= (1 << COM1A1);
		}
		else if (Copy_u8Polarity == PWM_INVERTING)
		{
			TCCR1A |= ((1 << COM1A0) | (1 << COM1A1));
		}

		TCCR1B |= PWM_u8PrescalerBits_T0_T1[Copy_u8Prescaler];
		break;

		case PWM_CHANNEL_2:
		/* Clear configuration mapping for the secondary independent 8-bit engine */
		TCCR2 &= ~((1 << WGM20) | (1 << WGM21) | (1 << COM20) | (1 << COM21) | (1 << CS20) | (1 << CS21) | (1 << CS22));
		
		if (Copy_u8PWMMode == PWM_MODE_FAST)
		{
			TCCR2 |= ((1 << WGM20) | (1 << WGM21));
		}
		else if (Copy_u8PWMMode == PWM_MODE_PHASE_CORRECT)
		{
			TCCR2 |= (1 << WGM20);
		}

		if (Copy_u8Polarity == PWM_NON_INVERTING)
		{
			TCCR2 |= (1 << COM21);
		}
		else if (Copy_u8Polarity == PWM_INVERTING)
		{
			TCCR2 |= ((1 << COM20) | (1 << COM21));
		}

		TCCR2 |= PWM_u8PrescalerBits_T2[Copy_u8Prescaler];
		break;
	}
}


void PWM_voidSetCompareMatchValue(u8 Copy_u8Channel, u16 Copy_u16CompareValue)
{
	/* Direct injection mechanism shifting values into live shadow register systems */
	switch(Copy_u8Channel)
	{
		case PWM_CHANNEL_0:  OCR0  = (u8)Copy_u16CompareValue;  break;
		case PWM_CHANNEL_1A: OCR1A = Copy_u16CompareValue;      break;
		case PWM_CHANNEL_2:  OCR2  = (u8)Copy_u16CompareValue;  break;
	}
}

