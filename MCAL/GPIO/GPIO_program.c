/*
 * GPIO_program.c
 *
 * Created: 17-May-26 9:34:30 AM
 *  Author: Eyad Hatem
 */ 

/*********************************************************************************/
/* File:        GPIO_program.c                                                   */
/* Author:      Embedded Systems Level 5                                        */
/* Description: Low-Level Hardware Manipulation Source Code for ATmega32 GPIO.    */
/* Uses standard <avr/io.h> register naming definitions.            */
/*********************************************************************************/

#include <avr/io.h>
#include "GPIO_interface.h"

/* ============================================================================= */
/* IMPLEMENTATIONS: SINGLE-PIN OPERATION FUNCTIONS                       */
/* ============================================================================= */

/**
 * DESCRIPTION:
 * This function changes the data direction of a single specific pin inside a chosen port.
 * It translates software configurations (INPUT, OUTPUT, or INPUT_PULLUP) into low-level
 * hardware bit manipulations across the Data Direction Registers (DDRx) and Port Registers (PORTx).
 */
void GPIO_voidSetPinDirection(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Direction)
{
    /* Safety Guard: Verify that the pin index doesn't exceed the legal 8-bit boundary (0 to 7) */
    if (Copy_u8Pin <= GPIO_PIN7)
    {
        switch(Copy_u8Port)
        {
            case GPIO_PORTA:
                if (Copy_u8Direction == GPIO_OUTPUT)
                {
                    DDRA |= (1 << Copy_u8Pin);          /* Write 1 to DDR: Configures pin as an active OUTPUT */
                }
                else if (Copy_u8Direction == GPIO_INPUT)
                {
                    DDRA  &= ~(1 << Copy_u8Pin);        /* Write 0 to DDR: Configures pin as a standard INPUT */
                    PORTA &= ~(1 << Copy_u8Pin);        /* Write 0 to PORT: Ensures the internal pull-up is fully disabled */
                }
                else if (Copy_u8Direction == GPIO_INPUT_PULLUP)
                {
                    DDRA  &= ~(1 << Copy_u8Pin);        /* Write 0 to DDR: Sets pin character to INPUT mode */
                    PORTA |=  (1 << Copy_u8Pin);        /* Write 1 to PORT: Engages the internal silicon Pull-Up resistor (Rpu) */
                }
                break;

            case GPIO_PORTB:
                if (Copy_u8Direction == GPIO_OUTPUT)
                {
                    DDRB |= (1 << Copy_u8Pin);          /* Output Mode Configuration */
                }
                else if (Copy_u8Direction == GPIO_INPUT)
                {
                    DDRB  &= ~(1 << Copy_u8Pin);        /* Input Mode Configuration */
                    PORTB &= ~(1 << Copy_u8Pin);        /* Clear Pull-up resistor switch */
                }
                else if (Copy_u8Direction == GPIO_INPUT_PULLUP)
                {
                    DDRB  &= ~(1 << Copy_u8Pin);        /* Input Mode Configuration */
                    PORTB |=  (1 << Copy_u8Pin);        /* Enable internal pull-up circuitry */
                }
                break;

            case GPIO_PORTC:
                if (Copy_u8Direction == GPIO_OUTPUT)
                {
                    DDRC |= (1 << Copy_u8Pin);          /* Output Mode Configuration */
                }
                else if (Copy_u8Direction == GPIO_INPUT)
                {
                    DDRC  &= ~(1 << Copy_u8Pin);        /* Input Mode Configuration */
                    PORTC &= ~(1 << Copy_u8Pin);        /* Clear Pull-up resistor switch */
                }
                else if (Copy_u8Direction == GPIO_INPUT_PULLUP)
                {
                    DDRC  &= ~(1 << Copy_u8Pin);        /* Input Mode Configuration */
                    PORTC |=  (1 << Copy_u8Pin);        /* Enable internal pull-up circuitry */
                }
                break;

            case GPIO_PORTD:
                if (Copy_u8Direction == GPIO_OUTPUT)
                {
                    DDRD |= (1 << Copy_u8Pin);          /* Output Mode Configuration */
                }
                else if (Copy_u8Direction == GPIO_INPUT)
                {
                    DDRD  &= ~(1 << Copy_u8Pin);        /* Input Mode Configuration */
                    PORTD &= ~(1 << Copy_u8Pin);        /* Clear Pull-up resistor switch */
                }
                else if (Copy_u8Direction == GPIO_INPUT_PULLUP)
                {
                    DDRD  &= ~(1 << Copy_u8Pin);        /* Input Mode Configuration */
                    PORTD |=  (1 << Copy_u8Pin);        /* Enable internal pull-up circuitry */
                }
                break;
        }
    }
}

/**
 * DESCRIPTION:
 * Drives an electrical output level to an existing output pin.
 * Writing a GPIO_HIGH flags the chip to generate 5 Volts out of the pin pad.
 * Writing a GPIO_LOW bridges the physical internal pathway straight down to Ground (0V).
 */
void GPIO_voidWritePinValue(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Value)
{
    /* Safety Guard: Confirm target pin address is valid */
    if (Copy_u8Pin <= GPIO_PIN7)
    {
        switch(Copy_u8Port)
        {
            case GPIO_PORTA:
                if (Copy_u8Value == GPIO_HIGH)
                {
                    PORTA |= (1 << Copy_u8Pin);         /* Sets target bit to 1: Delivers ~5.0V out */
                }
                else
                {
                    PORTA &= ~(1 << Copy_u8Pin);        /* Clears target bit to 0: Drops line to 0V (GND) */
                }
                break;

            case GPIO_PORTB:
                if (Copy_u8Value == GPIO_HIGH)          { PORTB |=  (1 << Copy_u8Pin); }
                else                                    { PORTB &= ~(1 << Copy_u8Pin); }
                break;

            case GPIO_PORTC:
                if (Copy_u8Value == GPIO_HIGH)          { PORTC |=  (1 << Copy_u8Pin); }
                else                                    { PORTC &= ~(1 << Copy_u8Pin); }
                break;

            case GPIO_PORTD:
                if (Copy_u8Value == GPIO_HIGH)          { PORTD |=  (1 << Copy_u8Pin); }
                else                                    { PORTD &= ~(1 << Copy_u8Pin); }
                break;
        }
    }
}

/**
 * DESCRIPTION:
 * Reads and samples the real-world physical voltage level existing on an input pin leg.
 * It monitors the Input Pins Register (PINx) and isolates the target bit using a bitwise AND mask.
 * RETURNS: 1 (GPIO_HIGH) if voltage is detected, or 0 (GPIO_LOW) if line is pulled down to Ground.
 */
u8 GPIO_u8ReadPinValue(u8 Copy_u8Port, u8 Copy_u8Pin)
{
    u8 Local_u8ResultValue = GPIO_LOW; /* Default local register variable initialization */

    /* Check boundary access safety constraints */
    if (Copy_u8Pin <= GPIO_PIN7)
    {
        switch(Copy_u8Port)
        {
            case GPIO_PORTA:
                /* Read PINA, apply a bitmask to wipe away other pin data, and evaluate the specific bit */
                if ((PINA & (1 << Copy_u8Pin)) == 0)    { Local_u8ResultValue = GPIO_LOW;  }
                else                                    { Local_u8ResultValue = GPIO_HIGH; }
                break;

            case GPIO_PORTB:
                if ((PINB & (1 << Copy_u8Pin)) == 0)    { Local_u8ResultValue = GPIO_LOW;  }
                else                                    { Local_u8ResultValue = GPIO_HIGH; }
                break;

            case GPIO_PORTC:
                if ((PINC & (1 << Copy_u8Pin)) == 0)    { Local_u8ResultValue = GPIO_LOW;  }
                else                                    { Local_u8ResultValue = GPIO_HIGH; }
                break;

            case GPIO_PORTD:
                if ((PIND & (1 << Copy_u8Pin)) == 0)    { Local_u8ResultValue = GPIO_LOW;  }
                else                                    { Local_u8ResultValue = GPIO_HIGH; }
                break;
        }
    }
    return Local_u8ResultValue; /* Send isolated high/low response back to caller application */
}

/**
 * DESCRIPTION:
 * Inverts the current logical and electrical state of a chosen output pin.
 * Using a bitwise XOR function (`^`), if the pin is currently outputting 5V, it drops to 0V.
 * If the pin is sitting at 0V, it instantly flips high to 5V.
 */
void GPIO_voidTogglePinValue(u8 Copy_u8Port, u8 Copy_u8Pin)
{
    if (Copy_u8Pin <= GPIO_PIN7)
    {
        switch(Copy_u8Port)
        {
            case GPIO_PORTA: PORTA ^= (1 << Copy_u8Pin); break; /* XOR flip operation on Port A */
            case GPIO_PORTB: PORTB ^= (1 << Copy_u8Pin); break; /* XOR flip operation on Port B */
            case GPIO_PORTC: PORTC ^= (1 << Copy_u8Pin); break; /* XOR flip operation on Port C */
            case GPIO_PORTD: PORTD ^= (1 << Copy_u8Pin); break; /* XOR flip operation on Port D */
        }
    }
}


/* ============================================================================= */
/* IMPLEMENTATIONS: WHOLE-PORT OPERATION FUNCTIONS                       */
/* ============================================================================= */

/**
 * DESCRIPTION:
 * Configures the direction parameters for an entire 8-bit wide port simultaneously.
 * Instead of dealing with separate bits individually, it overrides the whole DDRx byte instantly.
 * Useful values: 0xFF (configures all 8 pins as Outputs), 0x00 (configures all 8 pins as Inputs).
 */
void GPIO_voidSetPortDirection(u8 Copy_u8Port, u8 Copy_u8Direction)
{
    switch(Copy_u8Port)
    {
        case GPIO_PORTA: DDRA = Copy_u8Direction; break; /* Write direction configuration byte to DDRA register */
        case GPIO_PORTB: DDRB = Copy_u8Direction; break; /* Write direction configuration byte to DDRB register */
        case GPIO_PORTC: DDRC = Copy_u8Direction; break; /* Write direction configuration byte to DDRC register */
        case GPIO_PORTD: DDRD = Copy_u8Direction; break; /* Write direction configuration byte to DDRD register */
    }
}

/**
 * DESCRIPTION:
 * Overwrites an entire 8-bit port register (PORTx) with a new signal configuration layout.
 * This assigns logical levels to all 8 output pins in a single clock execution cycle.
 * This is critical for high-speed components, parallel data buses, and driving 7-Segment displays.
 */
void GPIO_voidWritePortValue(u8 Copy_u8Port, u8 Copy_u8Value)
{
    switch(Copy_u8Port)
    {
        case GPIO_PORTA: PORTA = Copy_u8Value; break; /* Direct parallel assignment to PORTA hardware latch */
        case GPIO_PORTB: PORTB = Copy_u8Value; break; /* Direct parallel assignment to PORTB hardware latch */
        case GPIO_PORTC: PORTC = Copy_u8Value; break; /* Direct parallel assignment to PORTC hardware latch */
        case GPIO_PORTD: PORTD = Copy_u8Value; break; /* Direct parallel assignment to PORTD hardware latch */
    }
}