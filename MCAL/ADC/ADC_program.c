/*********************************************************************************/
/* File:        ADC_program.c                                                    */
/* Author:      Embedded Systems Level 5                                         */
/* Description: Low-Level Hardware Manipulation Source Code for ATmega32 ADC.    */
/* Handles reference routing, prescaler scaling, dual-register reads and the ISR. */
/*********************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ADC_interface.h"

/* Null Definition for defensive callback initialization guards */
#define NULL    ((void*)0)

/* Mask covering the five multiplexer select bits MUX4:0 inside ADMUX */
#define ADC_CHANNEL_MASK    0x1F

/* Private storage for the registered upper-layer conversion-complete callback */
static void (*ADC_pvCallBackNotification)(void) = NULL;

/* Private holding cell for the latest reading captured by the background ISR */
static volatile u16 ADC_u16LastReading = 0;

/* ============================================================================= */
/* IMPLEMENTATIONS: PERIPHERAL CONFIGURATION AND CONTROL FUNCTIONS               */
/* ============================================================================= */

void ADC_voidInit(u8 Copy_u8Reference, u8 Copy_u8Prescaler)
{
    /* Set the reference voltage source using the top two bits of ADMUX (REFS1:0). */
    /* The result stays right-adjusted, so a full 0..1023 value lands in ADCL/ADCH. */
    ADMUX = (u8)(Copy_u8Reference << REFS0);

    /* Load the clock prescaler into the low three bits of ADCSRA (ADPS2:0). */
    ADCSRA = (Copy_u8Prescaler & 0x07);

    /* Power on the ADC by raising the enable bit (ADEN). */
    ADCSRA |= (1 << ADEN);
}


u16 ADC_u16Read(u8 Copy_u8Channel)
{
    u16 Local_u16Result = 0;

    /* Point the multiplexer at the chosen channel, keeping the reference bits. */
    ADMUX = (u8)((ADMUX & ~ADC_CHANNEL_MASK) | (Copy_u8Channel & ADC_CHANNEL_MASK));

    /* Start a single reading by raising the start bit (ADSC). */
    ADCSRA |= (1 << ADSC);

    /* Wait while the hardware works. ADSC stays high until the reading is done. */
    while ((ADCSRA & (1 << ADSC)) != 0)
    {
        /* just waiting for the conversion to finish */
    }

    /* Read the low byte first, then the high byte. The chip requires this order. */
    Local_u16Result  = ADCL;
    Local_u16Result |= (u16)(ADCH << 8);

    return Local_u16Result;
}


u16 ADC_u16ReadVoltage(u8 Copy_u8Channel, u16 Copy_u16RefMilliVolts)
{
    u16 Local_u16Counts = 0;
    u32 Local_u32MilliVolts = 0;

    /* Take a normal reading (0..1023). */
    Local_u16Counts = ADC_u16Read(Copy_u8Channel);

    /* Convert it to millivolts: voltage = counts * reference / 1023. */
    /* A 32-bit variable keeps the multiplication from overflowing. */
    Local_u32MilliVolts = ((u32)Local_u16Counts * Copy_u16RefMilliVolts) / 1023;

    return (u16)Local_u32MilliVolts;
}


void ADC_voidStartConversionAsync(u8 Copy_u8Channel)
{
    /* Point the multiplexer at the chosen channel. */
    ADMUX = (u8)((ADMUX & ~ADC_CHANNEL_MASK) | (Copy_u8Channel & ADC_CHANNEL_MASK));

    /* Turn on the conversion-complete interrupt so the CPU does not have to wait. */
    ADCSRA |= (1 << ADIE);

    /* Start the reading in the background. */
    ADCSRA |= (1 << ADSC);
}


void ADC_voidSetCallBack(void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        ADC_pvCallBackNotification = Copy_pvCallBackFunc;
    }
}


u16 ADC_u16GetLastReading(void)
{
    return ADC_u16LastReading;
}

/* ============================================================================= */
/* INTERRUPT SERVICE ROUTINE (ISR) - ADC CONVERSION COMPLETE VECTOR              */
/* ============================================================================= */

/**
 * @brief Runs automatically when a background reading finishes.
 * @note  Stores the result (low byte then high byte) and calls the user callback.
 */
ISR(ADC_vect)
{
    /* Save the new reading, keeping the required low-then-high read order. */
    ADC_u16LastReading  = ADCL;
    ADC_u16LastReading |= (u16)(ADCH << 8);

    /* Tell the application a fresh value is ready, if a callback was set. */
    if (ADC_pvCallBackNotification != NULL)
    {
        ADC_pvCallBackNotification();
    }
}
