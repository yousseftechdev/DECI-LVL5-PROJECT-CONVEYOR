/*********************************************************************************/
/* File:        BUTTON_interface.h                                               */
/* Author:      Embedded Systems Level 5                                         */
/* Description: Button Hardware Abstraction Layer Public API                     */
/*********************************************************************************/

#ifndef BUTTON_INTERFACE_H_
#define BUTTON_INTERFACE_H_

#define BUTTON_PULLUP              0
#define BUTTON_PULLDOWN            1

/**
 * @brief Prepares physical input vectors and configures edge triggering parameters.
 */
void BUTTON_voidInitInterruptButton(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8InterruptChannel, u8 Copy_u8PullType);

#endif /* BUTTON_INTERFACE_H_ */