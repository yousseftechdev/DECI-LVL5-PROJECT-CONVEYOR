/*********************************************************************************/
/* File:        MOTOR_interface.h                                                */
/* Author:      Embedded Systems Level 5                                         */
/* Description: DC Motor Hardware Abstraction Layer Public API                   */
/*********************************************************************************/

#ifndef MOTOR_INTERFACE_H_
#define MOTOR_INTERFACE_H_

/* Predefined Speed Levels mapped directly to Raw 8-Bit PWM Duty Cycle Values */
#define MOTOR_SPEED_25_PERCENT     64   /* ~25% of 255 */
#define MOTOR_SPEED_50_PERCENT     127  /* ~50% of 255 */
#define MOTOR_SPEED_75_PERCENT     191  /* ~75% of 255 */
#define MOTOR_SPEED_100_PERCENT    255  /* 100% of 255 */

/**
 * @brief Configures direction control pins and sets up the PWM speed generation channel.
 */
void MOTOR_voidInit(void);

/**
 * @brief Dictates H-Bridge logic pins to spin the motor in a Clockwise orientation.
 */
void MOTOR_voidSetDirectionCW(void);

/**
 * @brief Dictates H-Bridge logic pins to spin the motor in a Counter-Clockwise orientation.
 */
void MOTOR_voidSetDirectionCCW(void);

/**
 * @brief Injects raw comparison match values down to the PWM engine to throttle motor speed.
 * @param Copy_u8SpeedValue Options: MOTOR_SPEED_25_PERCENT, MOTOR_SPEED_50_PERCENT, etc.
 */
void MOTOR_voidSetSpeed(u8 Copy_u8SpeedValue);

#endif /* MOTOR_INTERFACE_H_ */