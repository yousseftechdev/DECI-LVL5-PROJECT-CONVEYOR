/*********************************************************************************/
/* File:        EXTI_interface.h                                                 */
/* Author:      Embedded Systems Level 5                                         */
/* Description: External Interrupt (EXTI) MCAL Driver Public API Architecture     */
/*********************************************************************************/

#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_

/* Standard Data Type Definitions (Matching System Architecture) */
typedef unsigned char u8;

/* EXTI Hardware Line Channels Available on ATmega32 Hardware */
#define EXTI_INT0                0   /* Mapped physically to Pin PD2 */
#define EXTI_INT1                1   /* Mapped physically to Pin PD3 */
#define EXTI_INT2                2   /* Mapped physically to Pin PB2 */

/* EXTI Sense Control Modes (Trigger Condition Configuration Options) */
#define EXTI_LOW_LEVEL           0   /* Trigger continuously while pin is Low (0V) */
#define EXTI_LOGICAL_CHANGE      1   /* Trigger on any voltage toggle (Rising/Falling) */
#define EXTI_FALLING_EDGE        2   /* Trigger on a high-to-low electrical transition */
#define EXTI_RISING_EDGE         3   /* Trigger on a low-to-high electrical transition */

/* ============================================================================= */
/* PUBLIC API FUNCTION PROTOTYPES                                                */
/* ============================================================================= */

/**
 * @brief Initializes chosen EXTI channel with its target sense triggering mode.
 * @param Copy_u8InterruptLine Options: EXTI_INT0, EXTI_INT1, EXTI_INT2
 * @param Copy_u8SenseControl  Options: EXTI_LOW_LEVEL, EXTI_LOGICAL_CHANGE, etc.
 */
void EXTI_voidInit(u8 Copy_u8InterruptLine, u8 Copy_u8SenseControl);

/**
 * @brief Dynamically updates or reconfigures the sense trigger mode of a channel.
 * @param Copy_u8InterruptLine Options: EXTI_INT0, EXTI_INT1, EXTI_INT2
 * @param Copy_u8SenseControl  Options: EXTI_LOW_LEVEL, EXTI_LOGICAL_CHANGE, etc.
 */
void EXTI_voidSetSenseControl(u8 Copy_u8InterruptLine, u8 Copy_u8SenseControl);

/**
 * @brief Enables runtime peripheral interrupt routing for a specific hardware line.
 */
void EXTI_voidEnableInterrupt(u8 Copy_u8InterruptLine);

/**
 * @brief Disables runtime peripheral interrupt routing for a specific hardware line.
 */
void EXTI_voidDisableInterrupt(u8 Copy_u8InterruptLine);

/**
 * @brief Registration mechanism sending custom APP-layer execution pointers down to ISR.
 * @param Copy_u8InterruptLine Options: EXTI_INT0, EXTI_INT1, EXTI_INT2
 * @param Copy_pvCallBackFunc  Pointer to a hardware-agnostic callback function (void arguments)
 */
void EXTI_voidSetCallBack(u8 Copy_u8InterruptLine, void (*Copy_pvCallBackFunc)(void));

#endif /* EXTI_INTERFACE_H_ */

