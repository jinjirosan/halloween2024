# Halloween 2024 Props Collection

A collection of Arduino-based animatronic and LED props for Halloween 2024, featuring servo-controlled eyes and LED effects.

## Components

### 1. Witch's Kettle Eye
Single-eye animatronic controller for a witch's cauldron/kettle prop.
- Servo-controlled eye movement
- LED illumination
- Multiple operation modes (Test, Operation, Off)
- Smooth movement patterns

**Hardware:**
- Arduino Nano Every Board
- SG90 Servo
- High-brightness LED
- 2x SPDT switches
- 5V power supply

### 2. Witchdoctor Hat
Multi-eye animatronic controller for a witchdoctor hat prop with 5 independently controlled eyes.
- PCA9685 PWM controller
- 5 synchronized servo/LED pairs
- Test and operation modes
- Individual eye movement patterns

**Hardware:**
- Arduino Nano Every Board
- PCA9685 16-channel PWM controller
- 5x SG90 Servos
- 5x High-brightness LEDs
- 2x SPDT switches
- 5V power supply

### 3. Witchdoctor Staff
LED strip controller for magical staff effects.
- Multiple LED patterns
- Synchronized animations
- Switch-controlled modes

**Hardware:**
- Adafruit Metro Mini v2
- Pro Trinket Lipoly backpack
- WS2812B LED strips (60 LEDs/m)
- Flora Neopixel V2
- 3.7V LiPo battery (1800mAh or 4400mAh)
- Multiple switches/buttons

## Installation

1. Install the Arduino IDE
2. Install required libraries:
   - Servo.h
   - Wire.h
   - Adafruit_PWMServoDriver.h
   - Adafruit_NeoPixel.h
3. Connect hardware according to pin definitions in each sketch
4. Upload corresponding sketch to each device

## Usage

### Witch's Kettle Eye
- **Test Mode (A1 LOW)**: Continuous sweep with LED blinking
- **Operation Mode (A0 LOW)**: Choreographed movement pattern
- **Off Mode (Both HIGH)**: Centered position, dim LED

### Witchdoctor Hat
- **Test Mode (D3 LOW)**: Synchronized eye movements
- **Operation Mode (D2 LOW)**: Individual eye patterns
- **Off Mode (Both HIGH)**: Centered position, dim LEDs

### Witchdoctor Staff
- Multiple pattern modes controlled via switches
- Battery-powered operation
- Synchronized LED animations

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.
