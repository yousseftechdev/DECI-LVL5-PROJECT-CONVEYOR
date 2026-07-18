# Hardware Component Roles

- ATmega32: Main controller that runs the conveyor state machine, ADC speed control, EXTI event handling, and UART logging.
- DC motor: Acts as the conveyor belt actuator and is driven by the H-bridge.
- H-bridge: Converts the MCU direction and PWM signals into motor drive signals for forward and reverse motion.
- Potentiometer: Provides a variable analog input to the ADC so the system can change the conveyor speed.
- Start button: Starts the conveyor when pressed and the system is idle.
- Stop button: Stops the conveyor when pressed during runtime.
- Emergency button: Forces the machine into an emergency-stop state until reset.
- Object sensor button: Simulates object detection events for counting.
- Status LEDs: Show running, idle, and fault states visually.
- UART terminal: Displays system messages such as startup, motor state changes, speed changes, object counts, and emergency stop events.
