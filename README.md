# ADC-Controlled Interactive Ping Pong Game(Nu-LB-NUC140)

## Overview
This project implements an interactive ping pong game using the **NUC140 microcontroller**. The game integrates hardware features like I2C communication, ADC input, PWM sound effects, and LCD display to create a real-time and engaging gameplay experience.
You can view the demo on YouTube [here.](https://youtu.be/TjInDzXRgKc)
## Features
- **I2C Communication**: Accesses a 24LC64 EEPROM to store game parameters such as random number seeds.
- **ADC Input**: Reads player controls from variable resistors to move paddles dynamically.
- **LCD Display**: Renders game graphics including paddles, ball, and score updates.
- **PWM Sound Effects**: Enhances the game experience with feedback sounds for events like wall hits, paddle hits, and scoring.
- **Real-Time Processing**: Utilizes interrupts and timers to maintain smooth gameplay.

## Hardware Requirements
- **Development Board**: Nu-LB-NUC140
- **Microcontroller**: NUC140VE3CN (LQFP100)
- **EEPROM**: 24LC64, connected via I2C1
- **Connections**:
  - I2C1-SCL: PA11
  - I2C1-SDA: PA10
- **Additional Components**:
  - LCD Display
  - Variable resistors for paddle control
  - Buzzer for sound output

## Software Features
- **Game Logic**: Implements paddle and ball movement, collision detection, scoring, and game-over conditions.
- **Interrupt-Driven Design**: Handles timer interrupts for gameplay timing and ADC interrupts for player input.
- **EEPROM Management**: Reads and writes data to EEPROM for persistent game state.
- **Graphical Rendering**: Draws sprites and text on the LCD display in real-time.

## File Structure
- `main.c`: Contains the main game logic and initialization code.
- `i2c.c`: Handles I2C communication for EEPROM access.
- `lcd.c`: Manages LCD display rendering.
- `sound.c`: Provides sound effect functionality using PWM.

## How to Run
1. **Set Up Hardware**:
   - Connect the 24LC64 EEPROM to the microcontroller via I2C1.
   - Attach variable resistors for paddle control.
   - Connect the LCD and buzzer to the respective pins.
2. **Compile and Flash**:
   - Use a compatible IDE and compiler for the NUC140 microcontroller.
   - Flash the compiled binary onto the microcontroller.
3. **Play**:
   - Adjust the variable resistors to control the paddles.
   - Observe the real-time game feedback on the LCD and through the buzzer.

## Game Rules
- Players control paddles using variable resistors.
- The ball bounces off the walls and paddles.
- A point is scored when the ball passes a paddle.
- The game ends when one player scores 3 points.

## Future Enhancements
- Add multiplayer support using UART for communication between two microcontrollers.
- Enhance graphics with more detailed sprites.
- Implement adjustable difficulty levels.

## Note
This project was part of the "Microprocessor" course taught by Professor Yi-Wen Wang in the Department of Computer Science and Information Engineering at Feng Chia University. The project received a perfect score.
