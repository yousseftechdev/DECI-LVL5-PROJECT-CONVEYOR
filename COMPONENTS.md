# Component Descriptions

- ATmega32 - the main microcontroller running the whole system. It reads the buttons and potentiometer, controls the motor through PWM, and logs everything over UART.

- DC Motor - drives the conveyor belt itself. Speed changes with the PWM duty cycle coming from the microcontroller.

- H-Bridge - sits between the microcontroller and the motor. It takes the low-power direction/PWM signals and switches the higher current needed to actually drive the motor.

- Potentiometer - used to manually set the belt speed. Its output goes into ADC0 and gets mapped into one of three speed levels.

- Start Button - starts the conveyor when pressed, as long as it isn't already running and there's no emergency active.

- Stop Button - stops the conveyor when pressed while it's running.

- Emergency Button - an interrupt-driven button (INT0) that immediately cuts the motor and locks the system. Only a reset clears it.

- Object Sensor Button - simulates an object passing on the belt. Wired to INT1, it increments the object counter whenever the belt is running.

- Status LEDs - three LEDs (running, idle, fault) that show the current state of the system at a glance.

- UART Terminal - the virtual serial terminal used to watch the system logs while the simulation runs.