// TheEvil Witchdoctor's Hat With 5 Eyes
// (2024) Voor m'n lieve guppie
//
// WitchdoctorHat.ino : v0.6-refactor 0.0.0 (dev release!)

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Define the left, right, and center angle limits for the servos
int left = 35;   // Leftmost position
int right = 130; // Rightmost position
int center = 87; // Center position
const int servoChannels[] = {0, 1, 2, 3, 4, 5}; // Servo channels
const int ledChannels[] = {8, 9, 10, 11, 12, 13};  // Corresponding LED channels
const int switchPinTestMode = 3;  // Pin for Test Mode switch (D3)
const int switchPinOperation = 2; // Pin for normal operation switch (D2)
bool isTestMode = false;          // Initially, not in test mode
bool isMoving = false;            // Initially, no movement

unsigned long previousMillisServo = 0;
unsigned long previousMillisLED = 0;
unsigned long previousMillisBlink = 0;       // For LED blinking
unsigned long previousMillisServoSelect = 0; // For changing active servos
const long intervalServo = 30;    // Adjusted interval for smooth servo movement
const long intervalLED = 50;      // Adjusted interval for smoother LED fade
const long blinkInterval = 500;   // LED blink interval (500ms)
const long changeActiveInterval = 5000;  // Change active servos every 5 seconds

int currentServoAngle[6] = {35, 35, 35, 35, 35, 35}; // Start all at the leftmost angle
int servoDirection[6] = {1, 1, 1, 1, 1, 1};          // 1 for sweeping right, -1 for sweeping left
bool ledState = false;                               // LED state for blinking

int activeServos[3] = {0, 1, 2};  // Initially, servos 0, 1, and 2 are active

void setup() {
  pinMode(switchPinTestMode, INPUT_PULLUP);  // Use internal pull-up resistor for test mode switch (D3)
  pinMode(switchPinOperation, INPUT_PULLUP); // Use internal pull-up resistor for normal operation switch (D2)
  Wire.begin();                              // Initialize I²C communication
  pwm.begin();                               // Initialize the PCA9685
  pwm.setPWMFreq(60);                        // Set frequency to 60Hz for both servos and LEDs

  // Center all servos at startup
  for (int i = 0; i < 6; i++) {
    centerServo(i);
  }

  // Set the initial brightness for LEDs
  for (int i = 0; i < 6; i++) {
    pwm.setPWM(ledChannels[i], 0, 0);  // Turn off LEDs initially
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if the test mode switch (D3) is LOW (ON)
  if (digitalRead(switchPinTestMode) == LOW) {
    isTestMode = true;  // Activate test mode
  } else {
    isTestMode = false; // Deactivate test mode
  }

  if (isTestMode) {
    // Run test mode logic: synchronized servos and LEDs
    if (currentMillis - previousMillisServo >= intervalServo) {
      previousMillisServo = currentMillis;
      updateServos(); // Synchronized servo movement
    }
    if (currentMillis - previousMillisLED >= intervalLED) {
      previousMillisLED = currentMillis;
      updateLEDs();   // Synchronized LED fading
    }
  } else {
    // Check if pin D2 is connected (LOW) or disconnected (HIGH)
    if (digitalRead(switchPinOperation) == HIGH) {
      // Pin D2 is disconnected: move servos to center and set LEDs to 50% brightness
      for (int i = 0; i < 6; i++) {
        centerServo(i); // Move each servo to the center position
        pwm.setPWM(ledChannels[i], 0, 2048); // Set each LED to 50% brightness
      }
    } else {
      // Normal operations with random selection of 3 servos
      if (currentMillis - previousMillisServoSelect >= changeActiveInterval) {
        previousMillisServoSelect = currentMillis;
        selectRandomServos(); // Change the active servos every 5 seconds
      }
      // Call separate functions for each servo-LED pair
      for (int i = 0; i < 3; i++) {
        operateServoLEDPair(activeServos[i], currentMillis); // Run specific logic for active servos
      }
    }
  }
}

// Function to center a specific servo
void centerServo(int servoIndex) {
  int pulseLength = map(center, 0, 180, 150, 600);  // Adjust pulse for the servo angle
  pwm.setPWM(servoChannels[servoIndex], 0, pulseLength);
}

// Function to update all servo positions in test mode
void updateServos() {
  for (int i = 0; i < 6; i++) {
    int pulseLength = map(currentServoAngle[i], 0, 180, 150, 600);  // Adjust pulse for servo angle
    pwm.setPWM(servoChannels[i], 0, pulseLength);

    // Update servo angle for sweeping in test mode
    currentServoAngle[i] += servoDirection[i];
    if (currentServoAngle[i] >= right || currentServoAngle[i] <= left) {
      servoDirection[i] = -servoDirection[i];  // Reverse direction at the edges
    }
  }
}

// Function to update LED brightness corresponding to servo positions in test mode
void updateLEDs() {
  for (int i = 0; i < 6; i++) {
    int brightness = map(currentServoAngle[i], left, right, 0, 4095);  // Brightness based on servo position
    pwm.setPWM(ledChannels[i], 0, brightness);
  }
}

// Function to select 3 random active servos every 5 seconds
void selectRandomServos() {
  int indices[] = {0, 1, 2, 3, 4, 5}; // All servo indices
  // Shuffle the array to randomly select the first 3 servos
  for (int i = 0; i < 6; i++) {
    int j = random(i, 6);  // Random index between i and the end of the array
    int temp = indices[i];
    indices[i] = indices[j];
    indices[j] = temp;
  }
  // Set the first 3 as active
  activeServos[0] = indices[0];
  activeServos[1] = indices[1];
  activeServos[2] = indices[2];
}

// Function to handle specific behavior for each servo-LED pair
void operateServoLEDPair(int servoIndex, unsigned long currentMillis) {
  switch (servoIndex) {
    case 0:
      // Fast movement and fast blinking for Servo 0 (Channel 0) and LED 0 (Channel 8)
      fastOperationServo0LED0(currentMillis);  // Custom function for Servo 0 and LED 0
      break;
    case 1:
      // Normal operation for Servo 1 (Channel 1) and LED 1 (Channel 9)
      normalOperationServo1LED1(currentMillis);  // Custom function for Servo 1 and LED 1
      break;
    case 2:
      // Custom behavior for Servo 2 and LED 2
      moveServoRandom(servoIndex);
      blinkLED(servoIndex, currentMillis);
      break;
    case 3:
      // Custom behavior for Servo 3 and LED 3
      moveServoRandom(servoIndex);
      blinkLED(servoIndex, currentMillis);
      break;
    case 4:
      // Custom behavior for Servo 4 and LED 4
      moveServoRandom(servoIndex);
      blinkLED(servoIndex, currentMillis);
      break;
    case 5:
      // Custom behavior for Servo 5 and LED 5
      moveServoRandom(servoIndex);
      blinkLED(servoIndex, currentMillis);
      break;
  }
}

// Function for fast movement and fast blinking for Servo 0 and LED 0
void fastOperationServo0LED0(unsigned long currentMillis) {
  static int servo0Angle = left;           // Start at the leftmost position
  static int servo0Direction = 1;          // 1 for sweeping right, -1 for sweeping left
  static bool led0State = false;           // Track the current state of the LED

  const long fastServoInterval = 10;       // Short interval for fast movement
  const long fastBlinkInterval = 100;      // Faster blink interval (100ms)

  // Handle fast servo movement
  if (currentMillis - previousMillisServo >= fastServoInterval) {
    previousMillisServo = currentMillis;
    servo0Angle += servo0Direction * 10;   // Move by steps of 10 for rapid movement

    if (servo0Angle >= right || servo0Angle <= left) {
      servo0Direction = -servo0Direction;  // Reverse direction at the edges
    }

    int pulseLength = map(servo0Angle, 0, 180, 150, 600);  // Map angle to pulse length
    pwm.setPWM(servoChannels[0], 0, pulseLength);          // Update Servo 0 position
  }

  // Handle fast LED blinking
  if (currentMillis - previousMillisBlink >= fastBlinkInterval) {
    previousMillisBlink = currentMillis;
    led0State = !led0State;  // Toggle LED state

    if (led0State) {
      pwm.setPWM(ledChannels[0], 0, 4095);  // Turn LED 0 on
    } else {
      pwm.setPWM(ledChannels[0], 0, 0);     // Turn LED 0 off
    }
  }
}

// Normal operation for Servo 1 (Channel 1) and LED 1 (Channel 9)
void normalOperationServo1LED1(unsigned long currentMillis) {
  // Servo 1 random movement logic
  if (currentMillis - previousMillisServo >= intervalServo) {
    previousMillisServo = currentMillis;
    int randomAngle = random(left, right);  // Random angle within the set boundaries
    int pulseLength = map(randomAngle, 0, 180, 150, 600);  // Map to pulse length for servo
    pwm.setPWM(servoChannels[1], 0, pulseLength);  // Update Servo 1
  }

  // LED 1 blinking logic
  if (currentMillis - previousMillisBlink >= blinkInterval) {
    previousMillisBlink = currentMillis;
    ledState = !ledState;  // Toggle LED state
    if (ledState) {
      pwm.setPWM(ledChannels[1], 0, 4095);  // Turn LED 1 on
    } else {
      pwm.setPWM(ledChannels[1], 0, 0);  // Turn LED 1 off
    }
  }
}

// Function to move servo randomly
void moveServoRandom(int servoIndex) {
  int randomAngle = random(left, right);  // Random angle within the set boundaries
  int pulseLength = map(randomAngle, 0, 180, 150, 600);  // Map to pulse length for servo
  pwm.setPWM(servoChannels[servoIndex], 0, pulseLength);  // Update servo
}

// Function to blink LED
void blinkLED(int servoIndex, unsigned long currentMillis) {
  static bool ledState = false;  // Track the current state of the LED

  if (currentMillis - previousMillisBlink >= blinkInterval) {
    previousMillisBlink = currentMillis;
    ledState = !ledState;  // Toggle the LED state

    if (ledState) {
      pwm.setPWM(ledChannels[servoIndex], 0, 4095);  // Turn the LED on
    } else {
      pwm.setPWM(ledChannels[servoIndex], 0, 0);  // Turn the LED off
    }
  }
}
