/*********************************************************************************/
/* File:        TIMER_program.c                                                  */
/* Author:      Embedded Systems Level 5                                         */
/* Description: Low-Level Hardware Manipulation Source Code for ATmega32 Timers.  */
/* Handles bit masking across control configurations and handles internal ISR vectors. */
/*********************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "TIMER_interface.h"

/* Null Definition for defensive callback initialization guards */
#define NULL    ((void*)0)

/* Private global array of function pointers storing registered upper-layer CTC callback addresses */
static void (*TIMER_pvCallBackNotificationCTC[3])(void) = {NULL, NULL, NULL};

/* Private Global Prescaler Mapping Arrays to translate Driver Encodings to explicit Hardware Bits */
static const u8 TIMER_u8PrescalerBits_T0_T1[6] = {0, 1, 2, 3, 4, 5}; /* T0/T1 Standard bits (CSn0, CSn1, CSn2) */
static const u8 TIMER_u8PrescalerBits_T2[6]    = {0, 1, 2, 4, 6, 7}; /* T2 Special Asynchronous bit structures */

/* ============================================================================= */
/* IMPLEMENTATIONS: PERIPHERAL CONFIGURATION AND CONTROL FUNCTIONS               */
/* ============================================================================= */

void TIMER_voidInit(u8 Copy_u8Channel, u8 Copy_u8Mode, u8 Copy_u8Prescaler)
{
    if (Copy_u8Prescaler <= TIMER_PRESCALER_1024)
    {
        switch(Copy_u8Channel)
        {
            case TIMER_CHANNEL_0:
                /* Mask away mode bits (WGM00, WGM01) and clock selection bits (CS00, CS01, CS02) */
                TCCR0 &= ~((1 << WGM00) | (1 << WGM01) | (1 << CS00) | (1 << CS01) | (1 << CS02));
                
                if (Copy_u8Mode == TIMER_MODE_NORMAL)
                {
                    /* Normal Mode operation bits remain cleared to 00 */
                    /* Disable execution module interrupt matching paths */
                    TIMSK &= ~(1 << OCIE0);
                }
                else if (Copy_u8Mode == TIMER_MODE_CTC)
                {
                    TCCR0 |= (1 << WGM01);      /* Establish CTC Mode Bit Setting */
                    TIMSK |= (1 << OCIE0);      /* Enable Peripheral Local Compare Match Interrupt */
                }
                
                /* Inject target prescaler clock choices out of execution mapping block array */
                TCCR0 |= TIMER_u8PrescalerBits_T0_T1[Copy_u8Prescaler];
                break;

            case TIMER_CHANNEL_1:
                /* Clear Mode control structures across split registers TCCR1A and TCCR1B */
                TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
                TCCR1B &= ~((1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS11) | (1 << CS12));
                
                if (Copy_u8Mode == TIMER_MODE_NORMAL)
                {
                    TIMSK &= ~(1 << OCIE1A);
                }
                else if (Copy_u8Mode == TIMER_MODE_CTC)
                {
                    TCCR1B |= (1 << WGM12);     /* CTC Mode with Top Value held at OCR1A */
                    TIMSK  |= (1 << OCIE1A);    /* Enable Local Channel Compare Match A Interrupt */
                }
                
                TCCR1B |= TIMER_u8PrescalerBits_T0_T1[Copy_u8Prescaler];
                break;

            case TIMER_CHANNEL_2:
                /* Clear configuration map registers for the secondary 8-bit module */
                TCCR2 &= ~((1 << WGM20) | (1 << WGM21) | (1 << CS20) | (1 << CS21) | (1 << CS22));
                
                if (Copy_u8Mode == TIMER_MODE_NORMAL)
                {
                    TIMSK &= ~(1 << OCIE2);
                }
                else if (Copy_u8Mode == TIMER_MODE_CTC)
                {
                    TCCR2 |= (1 << WGM21);
                    TIMSK |= (1 << OCIE2);
                }
                
                /* Note: Uses the specialized T2 clock selection hardware mapping footprint array */
                TCCR2 |= TIMER_u8PrescalerBits_T2[Copy_u8Prescaler];
                break;
        }
    }
}


void TIMER_voidSetPreloadValue(u8 Copy_u8Channel, u16 Copy_u16PreloadValue)
{
    switch(Copy_u8Channel)
    {
        case TIMER_CHANNEL_0: TCNT0 = (u8)Copy_u16PreloadValue;  break; /* Truncated to 8-bit boundary size */
        case TIMER_CHANNEL_1: TCNT1 = Copy_u16PreloadValue;      break; /* 16-bit register tracking allocation */
        case TIMER_CHANNEL_2: TCNT2 = (u8)Copy_u16PreloadValue;  break;
    }
}


void TIMER_voidSetCompareMatchValue(u8 Copy_u8Channel, u16 Copy_u16CompareValue)
{
    switch(Copy_u8Channel)
    {
        case TIMER_CHANNEL_0: OCR0 = (u8)Copy_u16CompareValue;   break;
        case TIMER_CHANNEL_1: OCR1A = Copy_u16CompareValue;      break; /* Outflows to Channel A Match Latch */
        case TIMER_CHANNEL_2: OCR2 = (u8)Copy_u16CompareValue;   break;
    }
}


void TIMER_voidPollOverflowDelay(u8 Copy_u8Channel, u16 Copy_u16PreloadValueValue)
{
    switch(Copy_u8Channel)
    {
        case TIMER_CHANNEL_0:
            TCNT0 = (u8)Copy_u16PreloadValueValue; /* Prime counting bucket starting point */
            while( (TIFR & (1 << TOV0)) == 0 )
            {
                /* Busy-Wait Block: Traps the program execution sequence counter 
                 * until physical hardware steps overflow the register to raise the TOV0 flag */
            }
            TIFR |= (1 << TOV0); /* Clear flag by writing a logical 1 down to the flag bit register location */
            break;

        case TIMER_CHANNEL_1:
            TCNT1 = Copy_u16PreloadValueValue;
            while( (TIFR & (1 << TOV1)) == 0 );
            TIFR |= (1 << TOV1);
            break;

        case TIMER_CHANNEL_2:
            TCNT2 = (u8)Copy_u16PreloadValueValue;
            while( (TIFR & (1 << TOV2)) == 0 );
            TIFR |= (1 << TOV2);
            break;
    }
}


void TIMER_voidSetCallBackCTC(u8 Copy_u8Channel, void (*Copy_pvCallBackFunc)(void))
{
    if (Copy_pvCallBackFunc != NULL)
    {
        TIMER_pvCallBackNotificationCTC[Copy_u8Channel] = Copy_pvCallBackFunc;
    }
}

/* ============================================================================= */
/* INTERRUPT SERVICE ROUTINES (ISR) - ATMEGA32 TIMING CHANNELS                    */
/* ============================================================================= */

/**
 * @brief ISR handler for Vector 10: Timer/Counter0 Compare Match Event
 */
ISR(TIMER0_COMP_vect)
{
    if (TIMER_pvCallBackNotificationCTC[TIMER_CHANNEL_0] != NULL)
    {
        TIMER_pvCallBackNotificationCTC[TIMER_CHANNEL_0]();
    }
}

/**
 * @brief ISR handler for Vector 7: Timer/Counter1 Compare Match A Event
 */
ISR(TIMER1_COMPA_vect)
{
    if (TIMER_pvCallBackNotificationCTC[TIMER_CHANNEL_1] != NULL)
    {
        TIMER_pvCallBackNotificationCTC[TIMER_CHANNEL_1]();
    }
}

/**
 * @brief ISR handler for Vector 4: Timer/Counter2 Compare Match Event
 */
ISR(TIMER2_COMP_vect)
{
    if (TIMER_pvCallBackNotificationCTC[TIMER_CHANNEL_2] != NULL)
    {
        TIMER_pvCallBackNotificationCTC[TIMER_CHANNEL_2]();
    }
}

