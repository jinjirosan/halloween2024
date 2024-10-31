// TheEvil Witchdoctor's Hat With 5 Eyes
// (2024) Voor m'n lieve guppie
//
// WitchdoctorHat.ino : v0.6-refactor c0.0.0 (dev release!)

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

unsigned long previousMillisServo[6] = {0, 0, 0, 0, 0, 0};
unsigned long previousMillisLED[6] = {0, 0, 0, 0, 0, 0};
unsigned long previousMillisBlink[6] = {0, 0, 0, 0, 0, 0};  // For LED blinking
const long intervalServo = 30;    // Adjusted interval for smooth servo movement
const long intervalLED = 50;      // Adjusted interval for smoother LED fade
const long blinkInterval = 500;   // LED blink interval (500ms)

int currentServoAngle[6] = {35, 35, 35, 35, 35, 35}; // Start all at the leftmost angle
int servoDirection[6] = {1, 1, 1, 1, 1, 1};          // 1 for sweeping right, -1 for sweeping left
bool ledState[6] = {false, false, false, false, false, false}; // LED state for blinking
int stage[6] = {0, 0, 0, 0, 0, 0};
unsigned long stageStart[6] = {0, 0, 0, 0, 0, 0};

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
    for (int i = 0; i < 6; i++) {
      if (currentMillis - previousMillisServo[i] >= intervalServo) {
        previousMillisServo[i] = currentMillis;
        updateServoTestMode(i); // Synchronized servo movement per servo
      }
      if (currentMillis - previousMillisLED[i] >= intervalLED) {
        previousMillisLED[i] = currentMillis;
        updateLEDTestMode(i); // Synchronized LED fading per LED
      }
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
      // Normal operations for all servos and LEDs
      for (int i = 0; i < 5; i++) { // Only iterate over connected servos (0-4)
        operateServoLEDPair(i, currentMillis); // Run specific logic for each servo
      }
    }
  }
}

// Function to center a specific servo
void centerServo(int servoIndex) {
  int pulseLength = map(center, 0, 180, 150, 600);  // Adjust pulse for the servo angle
  pwm.setPWM(servoChannels[servoIndex], 0, pulseLength);
}

// Function to update servo position in test mode
void updateServoTestMode(int servoIndex) {
  int pulseLength = map(currentServoAngle[servoIndex], 0, 180, 150, 600);  // Adjust pulse for servo angle
  pwm.setPWM(servoChannels[servoIndex], 0, pulseLength);

  // Update servo angle for sweeping in test mode
  currentServoAngle[servoIndex] += servoDirection[servoIndex];
  if (currentServoAngle[servoIndex] >= right || currentServoAngle[servoIndex] <= left) {
    servoDirection[servoIndex] = -servoDirection[servoIndex];  // Reverse direction at the edges
  }
}

// Function to update LED brightness corresponding to servo positions in test mode
void updateLEDTestMode(int servoIndex) {
  int brightness = map(currentServoAngle[servoIndex], left, right, 0, 4095);  // Brightness based on servo position
  pwm.setPWM(ledChannels[servoIndex], 0, brightness);
}

// Function to handle specific behavior for each servo-LED pair
void operateServoLEDPair(int servoIndex, unsigned long currentMillis) {
  switch (servoIndex) {
    case 0:
      operationModeCase0(servoIndex, currentMillis);
      break;
    case 1:
      //operationModeCase1(servoIndex, currentMillis);
      break;
    case 2:
      //operationModeCase2(servoIndex, currentMillis);
      break;
    case 3:
      //operationModeCase3(servoIndex, currentMillis);
      break;
    case 4:
      //operationModeCase4(servoIndex, currentMillis);
      break;
  }
}

// Function for operation mode case 0
void operationModeCase0(int servoIndex, unsigned long currentMillis) {
  switch (stage[servoIndex]) {
    case 0:
      // Center the eye for 3 seconds
      centerServo(servoIndex);
      pwm.setPWM(ledChannels[servoIndex], 0, 4095); // LED stays on
      if (currentMillis - stageStart[servoIndex] >= 3000) {
        stage[servoIndex] = 1;
        stageStart[servoIndex] = currentMillis;
      }
      break;
    case 1:
      // Move swiftly to the left
      int pulseLengthLeft = map(left, 0, 180, 150, 600);
      pwm.setPWM(servoChannels[servoIndex], 0, pulseLengthLeft);
      if (currentMillis - stageStart[servoIndex] >= 2000) {
        stage[servoIndex] = 2;
        stageStart[servoIndex] = currentMillis;
      }
      break;
    case 2:
      // Blink twice at the left boundary
      if (currentMillis - stageStart[servoIndex] < 1000) {
        blinkLED(servoIndex, currentMillis);
      } else {
        stage[servoIndex] = 3;
        stageStart[servoIndex] = currentMillis;
      }
      break;
    case 3:
      // Move slowly back to center
      int pulseLengthCenter = map(center, 0, 180, 150, 600);
      pwm.setPWM(servoChannels[servoIndex], 0, pulseLengthCenter); // Slow move back to center
      if (currentMillis - stageStart[servoIndex] >= 3000) {
        stage[servoIndex] = 4;
        stageStart[servoIndex] = currentMillis;
      }
      break;
    case 4:
      // Center for 3 seconds
      centerServo(servoIndex);
      if (currentMillis - stageStart[servoIndex] >= 3000) {
        stage[servoIndex] = 5;
        stageStart[servoIndex] = currentMillis;
      }
      break;
    case 5:
      // Move swiftly to the right
      int pulseLengthRight = map(right, 0, 180, 150, 600);
      pwm.setPWM(servoChannels[servoIndex], 0, pulseLengthRight);
      if (currentMillis - stageStart[servoIndex] >= 2000) {
        stage[servoIndex] = 6;
        stageStart[servoIndex] = currentMillis;
      }
      break;
    case 6:
      // Blink twice at the right boundary
      if (currentMillis - stageStart[servoIndex] < 1000) {
        blinkLED(servoIndex, currentMillis);
      } else {
        stage[servoIndex] = 7;
        stageStart[servoIndex] = currentMillis;
      }
      break;
    case 7:
      // Move slowly back to center
      pwm.setPWM(servoChannels[servoIndex], 0, pulseLengthCenter); // Slow move back to center
      if (currentMillis - stageStart[servoIndex] >= 3000) {
        stage[servoIndex] = 0;
        stageStart[servoIndex] = currentMillis;
      }
      break;
  }
}

// Function for operation mode case 1
void operationModeCase1(int servoIndex, unsigned long currentMillis) {
  if (stage[servoIndex] == 0) {
    // Move back and forth continuously
    if (currentMillis - previousMillisServo[servoIndex] >= intervalServo) {
      previousMillisServo[servoIndex] = currentMillis;
      int pulseLength = map(currentServoAngle[servoIndex], 0, 180, 150, 600);
      pwm.setPWM(servoChannels[servoIndex], 0, pulseLength);
      currentServoAngle[servoIndex] += servoDirection[servoIndex];
      if (currentServoAngle[servoIndex] >= right || currentServoAngle[servoIndex] <= left) {
        servoDirection[servoIndex] = -servoDirection[servoIndex];
      }
    }
    if (currentMillis - previousMillisLED[servoIndex] >= intervalLED) {
      previousMillisLED[servoIndex] = currentMillis;
      updateLEDTestMode(servoIndex);
    }
  }
}

// Function for operation mode case 2
void operationModeCase2(int servoIndex, unsigned long currentMillis) {
  if (stage[servoIndex] == 0) {
    // Fade LED in and out
    int brightness = map(currentMillis % 4000, 0, 2000, 0, 4095);
    pwm.setPWM(ledChannels[servoIndex], 0, brightness);
  }
}

// Function for operation mode case 3
void operationModeCase3(int servoIndex, unsigned long currentMillis) {
  if (stage[servoIndex] == 0) {
    // Center servo and keep LED on
    centerServo(servoIndex);
    pwm.setPWM(ledChannels[servoIndex], 0, 4095); // LED on
  }
}

// Function for operation mode case 4
void operationModeCase4(int servoIndex, unsigned long currentMillis) {
  if (stage[servoIndex] == 0) {
    // Center servo and keep LED on
    centerServo(servoIndex);
    pwm.setPWM(ledChannels[servoIndex], 0, 4095); // LED on
  }
}

// Function to blink LED
void blinkLED(int servoIndex, unsigned long currentMillis) {
  if (currentMillis - previousMillisBlink[servoIndex] >= blinkInterval) {
    previousMillisBlink[servoIndex] = currentMillis;
    ledState[servoIndex] = !ledState[servoIndex];  // Toggle the LED state

    if (ledState[servoIndex]) {
      pwm.setPWM(ledChannels[servoIndex], 0, 4095);  // Turn the LED on
    } else {
      pwm.setPWM(ledChannels[servoIndex], 0, 0);  // Turn the LED off
    }
  }
}
