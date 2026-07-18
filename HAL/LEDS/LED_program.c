#include "GPIO_interface.h"
#include "LED_interface.h"
#include "LED_CFG.h"

void LED_voidInit() {
	GPIO_voidSetPinDirection(LED_PORT, LED_RUNNING, GPIO_OUTPUT);
	GPIO_voidSetPinDirection(LED_PORT, LED_IDLE,    GPIO_OUTPUT);
	GPIO_voidSetPinDirection(LED_PORT, LED_FAULT,   GPIO_OUTPUT);
	LED_voidTurnOffAll();
}

void LED_voidTurnOnCW() {
	GPIO_voidWritePinValue(LED_PORT, LED_RUNNING, GPIO_HIGH);
}

void LED_voidTurnOffCW() {
	GPIO_voidWritePinValue(LED_PORT, LED_RUNNING, GPIO_LOW);
}

void LED_voidTurnOnIdle() {
	GPIO_voidWritePinValue(LED_PORT, LED_IDLE, GPIO_HIGH);
}

void LED_voidTurnOffIdle() {
	GPIO_voidWritePinValue(LED_PORT, LED_IDLE, GPIO_LOW);
}

void LED_voidTurnOnFault() {
	GPIO_voidWritePinValue(LED_PORT, LED_FAULT, GPIO_HIGH);
}

void LED_voidTurnOffFault() {
	GPIO_voidWritePinValue(LED_PORT, LED_FAULT, GPIO_LOW);
}

void LED_voidTurnOnAll() {
	GPIO_voidWritePinValue(LED_PORT, LED_RUNNING, GPIO_HIGH);
	GPIO_voidWritePinValue(LED_PORT, LED_IDLE,    GPIO_HIGH);
	GPIO_voidWritePinValue(LED_PORT, LED_FAULT,   GPIO_HIGH);
}

void LED_voidTurnOffAll() {
	GPIO_voidWritePinValue(LED_PORT, LED_RUNNING, GPIO_LOW);
	GPIO_voidWritePinValue(LED_PORT, LED_IDLE,    GPIO_LOW);
	GPIO_voidWritePinValue(LED_PORT, LED_FAULT,   GPIO_LOW);
	}