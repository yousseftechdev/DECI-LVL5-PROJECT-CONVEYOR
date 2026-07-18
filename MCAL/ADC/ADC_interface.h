/*********************************************************************************/
/* File:        ADC_interface.h                                                  */
/* Author:      Embedded Systems Level 5                                         */
/* Description: 10-Bit Successive Approximation ADC MCAL Driver Public API        */
/*********************************************************************************/

#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

/* Standard Data Type Definitions (Matching System Architecture) */
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u32;

/* ADC Single-Ended Input Channels mapped to physical Port A pins (ADC0..ADC7) */
#define ADC_CHANNEL_0            0   /* Physical Pin PA0 */
#define ADC_CHANNEL_1            1   /* Physical Pin PA1 */
#define ADC_CHANNEL_2            2   /* Physical Pin PA2 */
#define ADC_CHANNEL_3            3   /* Physical Pin PA3 */
#define ADC_CHANNEL_4            4   /* Physical Pin PA4 */
#define ADC_CHANNEL_5            5   /* Physical Pin PA5 */
#define ADC_CHANNEL_6            6   /* Physical Pin PA6 */
#define ADC_CHANNEL_7            7   /* Physical Pin PA7 */

/* Voltage Reference Source Selection (maps directly to REFS1:0 in ADMUX) */
#define ADC_REF_AREF            0   /* External voltage applied on the AREF pin */
#define ADC_REF_AVCC           1   /* System 5V supply rail (the usual choice)  */
#define ADC_REF_INTERNAL_2V56  3   /* Internal 2.56V reference for small signals */

/* TIMER Clock Prescaler Configurations (Hardware Divider for the ADC clock) */
/* Aim for an ADC clock between 50kHz and 200kHz. /128 is safe at 8MHz or 16MHz. */
#define ADC_PRESCALER_2        1   /* Divide system clock by 2   */
#define ADC_PRESCALER_4        2   /* Divide system clock by 4   */
#define ADC_PRESCALER_8        3   /* Divide system clock by 8   */
#define ADC_PRESCALER_16       4   /* Divide system clock by 16  */
#define ADC_PRESCALER_32       5   /* Divide system clock by 32  */
#define ADC_PRESCALER_64       6   /* Divide system clock by 64  */
#define ADC_PRESCALER_128      7   /* Divide system clock by 128 */

/* ============================================================================= */
/* PUBLIC API FUNCTION PROTOTYPES                                                */
/* ============================================================================= */

/**
 * @brief Switches the ADC on and sets the reference voltage and clock prescaler.
 * @param Copy_u8Reference   Options: ADC_REF_AREF, ADC_REF_AVCC, ADC_REF_INTERNAL_2V56
 * @param Copy_u8Prescaler   Options: ADC_PRESCALER_64, ADC_PRESCALER_128, etc.
 * @note  Call this once at the start of your program.
 */
void ADC_voidInit(u8 Copy_u8Reference, u8 Copy_u8Prescaler);

/**
 * @brief Reads one analog channel and waits for the result (the simple way).
 * @param Copy_u8Channel  Options: ADC_CHANNEL_0 .. ADC_CHANNEL_7
 * @return A 10-bit value from 0 (0 volts) to 1023 (full reference voltage).
 */
u16 ADC_u16Read(u8 Copy_u8Channel);

/**
 * @brief Converts a finished reading into a real voltage in millivolts.
 * @param Copy_u8Channel       Options: ADC_CHANNEL_0 .. ADC_CHANNEL_7
 * @param Copy_u16RefMilliVolts The reference value in mV (5000 for 5V, 2560 for 2.56V).
 * @return A voltage in millivolts. Example: 2500 means 2.5 volts.
 */
u16 ADC_u16ReadVoltage(u8 Copy_u8Channel, u16 Copy_u16RefMilliVolts);

/**
 * @brief Starts a reading in the background without making the CPU wait.
 * @param Copy_u8Channel  Options: ADC_CHANNEL_0 .. ADC_CHANNEL_7
 * @note  When the reading finishes, your callback function runs automatically.
 */
void ADC_voidStartConversionAsync(u8 Copy_u8Channel);

/**
 * @brief Registers the function that runs when a background reading is ready.
 * @param Copy_pvCallBackFunc Pointer to your function (takes no inputs, returns nothing).
 */
void ADC_voidSetCallBack(void (*Copy_pvCallBackFunc)(void));

/**
 * @brief Gives back the most recent value captured by the background reading.
 * @return The last 10-bit reading (0 to 1023).
 */
u16 ADC_u16GetLastReading(void);

#endif /* ADC_INTERFACE_H_ */
