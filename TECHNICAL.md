# Technical Documentation

## Hardware Specifications

### Power Requirements
- WitchKettleEye: 5V DC, ~500mA
- WitchdoctorHat: 5V DC, ~1.5A
- WitchdoctorStaff: 3.7V LiPo, 1800mAh or 4400mAh

### Servo Specifications
- Type: SG90 or compatible
- Operating voltage: 4.8V-6V
- Angular range: 35° to 130°
- Center position: 87°

### LED Specifications
- WitchKettleEye: Single high-brightness LED
- WitchdoctorHat: 5x high-brightness LEDs
- WitchdoctorStaff: WS2812B LED strips, 60 LEDs/m

## Pin Mappings

### WitchKettleEye
- Servo: A2
- LED: A3
- Test Mode Switch: A1
- Operation Switch: A0

### WitchdoctorHat
- I2C: SDA/SCL
- Test Mode Switch: D3
- Operation Switch: D2
- PCA9685 Channels:
  - Servos: 0-5
  - LEDs: 8-13

### WitchdoctorStaff
- LED Strip C6: Pin 6 (18 LEDs)
- LED Strip C7: Pin 7 (30 LEDs)
- Mode Switches: Pins 2,3,4

## Timing Parameters

### WitchKettleEye
- Servo interval: 20ms
- LED blink interval: 500ms
- Boundary pause: 2000ms
- Center pause: 4000ms

### WitchdoctorHat
- Servo interval: 30ms
- LED interval: 50ms
- Blink interval: 500ms

## Operation Modes

Detailed documentation of each mode and its behavior... 