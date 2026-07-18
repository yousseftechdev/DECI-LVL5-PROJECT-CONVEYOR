/*********************************************************************************/
/* File:        PWM_interface.h                                                  */
/* Author:      Embedded Systems Level 5                                         */
/* Description: Hardware PWM Waveform Generation MCAL Driver Public API          */
/*********************************************************************************/

#ifndef PWM_INTERFACE_H_
#define PWM_INTERFACE_H_

/* Standard Data Type Definitions */
typedef unsigned char u8;
typedef unsigned short int u16;

/* PWM Hardware Channels corresponding to physical waveform generation pins */
#define PWM_CHANNEL_0            0   /* Controls Pin PB3 (OC0)  */
#define PWM_CHANNEL_1A           1   /* Controls Pin PD5 (OC1A) */
#define PWM_CHANNEL_2            2   /* Controls Pin PD7 (OC2)  */

/* PWM Architecture Mode Configurations */
#define PWM_MODE_FAST            0   /* Single-slope high frequency modulation */
#define PWM_MODE_PHASE_CORRECT   1   /* Dual-slope phase-aligned modulation */

/* PWM Duty Cycle Output Polarity Modes */
#define PWM_NON_INVERTING        0   /* Clear on Match, Set at TOP (High Duty = High Voltage) */
#define PWM_INVERTING            1   /* Set on Match, Clear at TOP (High Duty = Low Voltage)  */

/* TIMER Clock Prescaler Configurations (Reused from standard timer definitions) */
#define PWM_PRESCALER_8          2   
#define PWM_PRESCALER_64         3   
#define PWM_PRESCALER_256        4   
#define PWM_PRESCALER_1024       5   

/* ============================================================================= */
/* PUBLIC API FUNCTION PROTOTYPES                                                */
/* ============================================================================= */

/**
 * @brief Configures and initializes the target hardware channel's PWM engine mode.
 * @param Copy_u8Channel    Options: PWM_CHANNEL_0, PWM_CHANNEL_1A, PWM_CHANNEL_2
 * @param Copy_u8PWMMode    Options: PWM_MODE_FAST, PWM_MODE_PHASE_CORRECT
 * @param Copy_u8Polarity   Options: PWM_NON_INVERTING, PWM_INVERTING
 * @param Copy_u8Prescaler  Options: PWM_PRESCALER_64, PWM_PRESCALER_256, etc.
 */
void PWM_voidInit(u8 Copy_u8Channel, u8 Copy_u8PWMMode, u8 Copy_u8Polarity, u8 Copy_u8Prescaler);

/**
 * @brief Sets the raw Compare Match registration limit to change the signal duty cycle.
 * @note Accepts up to 16 bits for Channel 1A; automatically truncated for 8-bit engines.
 */
void PWM_voidSetCompareMatchValue(u8 Copy_u8Channel, u16 Copy_u16CompareValue);

#endif /* PWM_INTERFACE_H_ */

