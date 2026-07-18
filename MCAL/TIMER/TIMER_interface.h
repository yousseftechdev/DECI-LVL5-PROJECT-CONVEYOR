/*********************************************************************************/
/* File:        TIMER_interface.h                                                */
/* Author:      Embedded Systems Level 5                                         */
/* Description: Hardware Timers MCAL Driver Public API Architecture               */
/*********************************************************************************/

#ifndef TIMER_INTERFACE_H_
#define TIMER_INTERFACE_H |

/* Standard Data Type Definitions (Matching System Architecture) */
typedef unsigned char u8;
typedef unsigned short int u16;

/* TIMER Hardware Peripheral Channels Available on ATmega32 Hardware */
#define TIMER_CHANNEL_0          0   /* 8-Bit Hardware Timer Module */
#define TIMER_CHANNEL_1          1   /* 16-Bit Hardware Timer Module */
#define TIMER_CHANNEL_2          2   /* 8-Bit Hardware Timer Module (Asynchronous Capable) */

/* TIMER Waveform Generation Modes Supported by this Driver Implementation */
#define TIMER_MODE_NORMAL        0   /* Standard Looping Sawtooth Wave Generation Profile */
#define TIMER_MODE_CTC           1   /* Clear Timer on Compare Match Frequency Mode */

/* TIMER Clock Prescaler Configuration Selections (Hardware Divider Gearbox) */
#define TIMER_PRESCALER_NONE     0   /* Timer Stopped (No Clock Source Connected) */
#define TIMER_PRESCALER_1        1   /* System Clock directly routed (No Division) */
#define TIMER_PRESCALER_8        2   /* Divide System Clock by 8 */
#define TIMER_PRESCALER_64       3   /* Divide System Clock by 64 */
#define TIMER_PRESCALER_256      4   /* Divide System Clock by 256 */
#define TIMER_PRESCALER_1024     5   /* Divide System Clock by 1024 */

/* ============================================================================= */
/* PUBLIC API FUNCTION PROTOTYPES                                                */
/* ============================================================================= */

/**
 * @brief Initializes a target timer channel with its core mode and prescaler configuration.
 * @param Copy_u8Channel    Options: TIMER_CHANNEL_0, TIMER_CHANNEL_1, TIMER_CHANNEL_2
 * @param Copy_u8Mode       Options: TIMER_MODE_NORMAL, TIMER_MODE_CTC
 * @param Copy_u8Prescaler  Options: TIMER_PRESCALER_8, TIMER_PRESCALER_64, etc.
 */
void TIMER_voidInit(u8 Copy_u8Channel, u8 Copy_u8Mode, u8 Copy_u8Prescaler);

/**
 * @brief Overwrites the foundational count value inside the live hardware tracking register.
 * @note Accepts up to 16 bits for Timer1 compatibility; automatically truncated safely for 8-bit channels.
 */
void TIMER_voidSetPreloadValue(u8 Copy_u8Channel, u16 Copy_u16PreloadValue);

/**
 * @brief Sets the target generation ceiling reference step inside the hardware match registers.
 */
void TIMER_voidSetCompareMatchValue(u8 Copy_u8Channel, u16 Copy_u16CompareValue);

/**
 * @brief Implements an atomic blocking hardware timing window utilizing polling flags.
 * @note Clears internal flags manually; forces a programmatic restart loop cycle window.
 */
void TIMER_voidPollOverflowDelay(u8 Copy_u8Channel, u16 Copy_u16PreloadValueValue);

/**
 * @brief Registration mechanism sending custom APP-layer execution pointers down to CTC ISRs.
 * @param Copy_pvCallBackFunc Pointer to a hardware-agnostic callback function (void arguments)
 */
void TIMER_voidSetCallBackCTC(u8 Copy_u8Channel, void (*Copy_pvCallBackFunc)(void));

#endif /* TIMER_INTERFACE_H_ */

