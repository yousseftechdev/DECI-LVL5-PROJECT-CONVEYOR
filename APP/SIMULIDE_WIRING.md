# SimulIDE Wiring Plan

1. ATmega32
   - PA0 -> potentiometer output for ADC speed input.
   - PB0 -> start button input, pull-up enabled.
   - PB1 -> stop button input, pull-up enabled.
   - PD2 -> emergency button input, EXTI INT0.
   - PD3 -> object sensor button input, EXTI INT1.
   - PD0 -> UART RX from terminal.
   - PD1 -> UART TX to terminal.
   - PB3 -> PWM output to H-bridge enable/speed input.
   - PD4 and PD5 -> H-bridge direction control pins.

2. Motor and H-bridge
   - Connect the H-bridge enable/speed pin to PB3.
   - Connect H-bridge IN1/IN2 to PD4/PD5.
   - Connect the H-bridge output pins to the DC motor.
   - Power the H-bridge from the simulation supply rails.

3. Status LEDs
   - Running LED -> PC0.
   - Idle LED -> PC1.
   - Fault LED -> PC2.

4. UART terminal
   - Connect terminal TX to PD0.
   - Connect terminal RX to PD1.

5. Buttons
   - Start and stop push buttons each connect to their respective MCU pins with pull-up resistors.
   - Emergency and object sensor buttons connect to PD2 and PD3 with pull-up resistors.
