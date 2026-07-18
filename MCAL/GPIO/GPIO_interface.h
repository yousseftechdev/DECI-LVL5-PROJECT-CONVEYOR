/*
 * GPIO_interface.h
 *
 * Created: 17-May-26 9:32:05 AM
 *  Author: Eyad Hatem
 */ 

/*********************************************************************************/
/* File:        GPIO_interface.h                                                 */
/* Author:      Embedded Systems Level 5                                  */
/* Description: Public API Constants, Configuration Macros, and Functional       */
/* Documentation for the ATmega32 GPIO Peripheral Driver Layer.     */
/*********************************************************************************/

#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

/* ============================================================================= */
/* TYPE DEFINITIONS                                                              */
/* ============================================================================= */
typedef unsigned char u8;

/* ============================================================================= */
/* CONFIGURATION MACROS & CONSTANTS                                              */
/* ============================================================================= */

/* Port Indexing Identifiers */
#define GPIO_PORTA              0
#define GPIO_PORTB              1
#define GPIO_PORTC              2
#define GPIO_PORTD              3

/* Individual Pin Indexing Identifiers */
#define GPIO_PIN0               0
#define GPIO_PIN1               1
#define GPIO_PIN2               2
#define GPIO_PIN3               3
#define GPIO_PIN4               4
#define GPIO_PIN5               5
#define GPIO_PIN6               6
#define GPIO_PIN7               7

/* Pin Directional Behavior Modes */
#define GPIO_INPUT              0  /* Standard high-impedance digital input      */
#define GPIO_OUTPUT             1  /* Active push-pull electronic switch output  */
#define GPIO_INPUT_PULLUP       2  /* Input mode with internal 30k Ohm Rpu active*/

/* Digital Logic Voltage Output levels */
#define GPIO_LOW                0  /* Pulls the pin down to 0V (Ground)          */
#define GPIO_HIGH               1  /* Pushes 5V out of the physical pin          */


/* ============================================================================= */
/*  SINGLE-PIN FUNCTION PROTOTYPES                                       */
/* ============================================================================= */

/**
 * @brief Configures the data direction mode of a single, specified physical pin.
 * * This function modifies the Data Direction Register (DDRx) and Port Register (PORTx)
 * to set up how a pin interacts with external hardware.
 * * @param Copy_u8Port      The target hardware port index. 
 * Expects: [GPIO_PORTA, GPIO_PORTB, GPIO_PORTC, or GPIO_PORTD]
 * @param Copy_u8Pin       The specific pin number within the selected port.
 * Expects: [GPIO_PIN0 to GPIO_PIN7]
 * @param Copy_u8Direction The electrical identity rule for the pin.
 * Expects:
 * - GPIO_INPUT        : Puts pin in high-resistance sensing mode.
 * - GPIO_OUTPUT       : Puts pin in active power driving mode.
 * - GPIO_INPUT_PULLUP : Sets as input and connects the internal 5V resistor.
 * @return void
 */
void GPIO_voidSetPinDirection(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Direction);

/**
 * @brief Drives a specific digital logic state (High/Low) onto a configured output pin.
 * * This function interacts directly with the physical output latch (PORTx) to control
 * current flow. Writing high pushes 5V outward; writing low drains the channel to 0V.
 * * @param Copy_u8Port  The target hardware port index.
 * Expects: [GPIO_PORTA, GPIO_PORTB, GPIO_PORTC, or GPIO_PORTD]
 * @param Copy_u8Pin   The specific pin number within the selected port.
 * Expects: [GPIO_PIN0 to GPIO_PIN7]
 * @param Copy_u8Value The targeted electrical voltage level.
 * Expects: [GPIO_HIGH (5V) or GPIO_LOW (0V / GND)]
 * @return void
 */
void GPIO_voidWritePinValue(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Value);

/**
 * @brief Samples and reads the instantaneous physical logic voltage present on an input pin leg.
 * * This function captures the state of the Input Pins Register (PINx) and evaluates whether
 * the voltage boundary matches a digital 1 or 0 by applying an isolated bitmask.
 * * @param Copy_u8Port The target hardware port index to sample.
 * Expects: [GPIO_PORTA, GPIO_PORTB, GPIO_PORTC, or GPIO_PORTD]
 * @param Copy_u8Pin  The specific pin number to examine.
 * Expects: [GPIO_PIN0 to GPIO_PIN7]
 * @return u8         Returns the logical voltage state currently observed on the physical hardware:
 * - 1 (GPIO_HIGH) : Voltage detected (~5V)
 * - 0 (GPIO_LOW)  : Ground level detected (0V)
 */
u8 GPIO_u8ReadPinValue(u8 Copy_u8Port, u8 Copy_u8Pin);

/**
 * @brief Inverts the existing logical and electrical state of an active output pin.
 * * This function applies a bitwise XOR operational toggle directly to the output register (PORTx).
 * If the physical pin is currently resting at 5V, it falls to 0V. If it is sitting at 0V, it switches to 5V.
 * * @param Copy_u8Port The target hardware port index.
 * Expects: [GPIO_PORTA, GPIO_PORTB, GPIO_PORTC, or GPIO_PORTD]
 * @param Copy_u8Pin  The specific pin number within the chosen port to toggle.
 * Expects: [GPIO_PIN0 to GPIO_PIN7]
 * @return void
 */
void GPIO_voidTogglePinValue(u8 Copy_u8Port, u8 Copy_u8Pin);


/* ============================================================================= */
/* WHOLE-PORT FUNCTION PROTOTYPES                                       */
/* ============================================================================= */

/**
 * @brief Configures the directional configuration mapping for an entire 8-bit port at once.
 * * Instead of altering individual channels sequentially, this function performs a full parallel
 * override of the target Data Direction Register (DDRx). 
 * * @param Copy_u8Port      The target hardware port index to configure.
 * Expects: [GPIO_PORTA, GPIO_PORTB, GPIO_PORTC, or GPIO_PORTD]
 * @param Copy_u8Direction An 8-bit mask mapping the direction patterns across all pins simultaneously.
 * Examples:
 * - 0xFF : Configures all 8 port pins as OUTPUTS.
 * - 0x00 : Configures all 8 port pins as INPUTS.
 * - 0xF0 : Configures pins 4-7 as Outputs, and pins 0-3 as Inputs.
 * @return void
 */
void GPIO_voidSetPortDirection(u8 Copy_u8Port, u8 Copy_u8Direction);

/**
 * @brief Writes an entire 8-bit raw configuration layout to a designated port latch instantly.
 * * This performs a direct parallel assignment to the target Port Register (PORTx), modifying
 * all 8 output lines within a single CPU clock execution cycle. This is an efficient approach 
 * for parallel communications buses and driving multi-segment hardware screens.
 * * @param Copy_u8Port  The target hardware port index to overwrite.
 * Expects: [GPIO_PORTA, GPIO_PORTB, GPIO_PORTC, or GPIO_PORTD]
 * @param Copy_u8Value The explicit 8-bit digital value pattern to assert onto the physical channels.
 * Example: 
 * - 0x55 (Binary: 01010101) turns on every alternating odd pin.
 * @return void
 */
void GPIO_voidWritePortValue(u8 Copy_u8Port, u8 Copy_u8Value);

#endif /* GPIO_INTERFACE_H_ */