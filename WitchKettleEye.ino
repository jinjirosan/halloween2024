// The Eye of the Witch's Kettle
// (2024) Voor m'n lieve guppie
//
// WitchKettleEye.ino : v1.0-refactor 0.0.0 (Operational release!)

#include <Servo.h>

// Define servo and LED parameters
Servo myServo;
const int servoPin = A2;  // Pin for the servo
const int ledPin = A3;    // Pin for the LED

// Define the left, right, and center angle limits for the servo
int left = 35;   // Leftmost position
int right = 130; // Rightmost position
int center = 87; // Center position

// Define the test mode and operation mode switches
const int switchPinTestMode = A1;  // Pin for Test Mode switch (A1)
const int switchPinOperation = A0; // Pin for normal operation switch (A0)
bool ledState = false;             // LED state for blinking

// Timing variables
unsigned long previousMillisServo = 0;
unsigned long previousMillisBlink = 0;
unsigned long previousMillisPause = 0;
const long intervalServo = 20;      // Interval for smooth servo movement
const long blinkInterval = 500;     // LED blink interval (500ms)
long testModeIntervalServo = 50;    // Separate interval for Test Mode sweeping speed

// Test Mode variables
int testModeServoAngle = left;    // Start at the leftmost angle for Test Mode
int testModeDirection = 1;        // 1 for sweeping right, -1 for sweeping left
int stepSize = 5;                 // Servo movement step size for Test Mode

// Duration variables
const int boundaryPauseDuration = 2000; // 2 seconds at boundaries (modifiable)
const int centerPauseDuration = 4000;   // 4 seconds at the center (modifiable)

// Operation Mode variables
int operationServoAngle = center; // Start at the center for Operation Mode

void setup() {
  pinMode(switchPinTestMode, INPUT_PULLUP);  // Use internal pull-up resistor for test mode switch (A1)
  pinMode(switchPinOperation, INPUT_PULLUP); // Use internal pull-up resistor for normal operation switch (A0)
  pinMode(ledPin, OUTPUT);                   // Set LED pin as output

  // Attach the servo to the specified pin
  myServo.attach(servoPin);
  myServo.write(center);  // Center the servo at startup

  // Turn off the LED initially
  analogWrite(ledPin, 0);
}

void loop() {
  unsigned long currentMillis = millis();

  // Check the switch states for Off Mode, Test Mode, and Operation Mode
  bool testModeActive = digitalRead(switchPinTestMode) == LOW;
  bool operationModeActive = digitalRead(switchPinOperation) == LOW;

  if (!testModeActive && !operationModeActive) {
    // Off Mode: Both switches are HIGH
    myServo.write(center);
    analogWrite(ledPin, 128); // Set LED to 50% brightness
  } else if (testModeActive && !operationModeActive) {
    // Test Mode: A1 is LOW and A0 is HIGH
    if (currentMillis - previousMillisServo >= testModeIntervalServo) {
      previousMillisServo = currentMillis;
      testModeServoSweep(); // Update servo position for sweeping motion
    }
    if (currentMillis - previousMillisBlink >= blinkInterval) {
      previousMillisBlink = currentMillis;
      toggleLED();   // Toggle LED state
    }
  } else if (!testModeActive && operationModeActive) {
    // Operation Mode: A0 is LOW and A1 is HIGH
    operationMode(currentMillis);
  }
}

// Function to handle Test Mode sweeping motion for the servo
void testModeServoSweep() {
  testModeServoAngle += stepSize * testModeDirection;

  // Reverse direction at the boundaries
  if (testModeServoAngle >= right || testModeServoAngle <= left) {
    testModeDirection = -testModeDirection;
  }

  // Update the servo position for Test Mode
  myServo.write(testModeServoAngle);
}

// Function for Operation Mode behavior
void operationMode(unsigned long currentMillis) {
  static int phase = 0; // 0: Move to left, 1: Pause at left, 2: Move to center, 3: Pause at center, 4: Move to right, 5: Pause at right, 6: Move to center
  static bool ledBlinked = false;

  switch (phase) {
    case 0: // Move to left boundary
      analogWrite(ledPin, 255); // Keep LED on during movement
      if (currentMillis - previousMillisServo >= intervalServo / 2) {
        previousMillisServo = currentMillis;
        moveToPosition(left); // Fast to left
        if (operationServoAngle == left) {
          phase = 1;
          ledBlinked = false;
          previousMillisPause = currentMillis;
        }
      }
      break;

    case 1: // Pause at left boundary, blink LED
      blinkLEDNonBlocking(2, currentMillis);
      if (currentMillis - previousMillisPause >= boundaryPauseDuration) {
        phase = 2;
      }
      break;

    case 2: // Move to center faster
      analogWrite(ledPin, 255); // Keep LED on during movement
      if (currentMillis - previousMillisServo >= intervalServo * 2) {
        previousMillisServo = currentMillis;
        moveToPosition(center); // Slow to center
        if (operationServoAngle == center) {
          analogWrite(ledPin, 0); // Turn LED off at center
          phase = 3;
          previousMillisPause = currentMillis;
        }
      }
      break;

    case 3: // Pause at center
      if (currentMillis - previousMillisPause >= centerPauseDuration) {
        phase = 4;
      }
      break;

    case 4: // Move to right boundary fast
      analogWrite(ledPin, 255); // Keep LED on during movement
      if (currentMillis - previousMillisServo >= intervalServo / 2) {
        previousMillisServo = currentMillis;
        moveToPosition(right); // Fast to right
        if (operationServoAngle == right) {
          phase = 5;
          ledBlinked = false;
          previousMillisPause = currentMillis;
        }
      }
      break;

    case 5: // Pause at right boundary, blink LED
      blinkLEDNonBlocking(2, currentMillis);
      if (currentMillis - previousMillisPause >= boundaryPauseDuration) {
        phase = 6;
      }
      break;

    case 6: // Move to center faster
      analogWrite(ledPin, 255); // Keep LED on during movement
      if (currentMillis - previousMillisServo >= intervalServo * 2) {
        previousMillisServo = currentMillis;
        moveToPosition(center); // Slow to center
        if (operationServoAngle == center) {
          analogWrite(ledPin, 0); // Turn LED off at center
          phase = 7;
          previousMillisPause = currentMillis;
        }
      }
      break;

    case 7: // Pause at the center to ensure completion of the cycle
      if (currentMillis - previousMillisPause >= centerPauseDuration) {
        phase = 0; // Restart the sequence
      }
      break;
  }
}

// Function to move servo to a specified position in Operation Mode with easing
void moveToPosition(int targetAngle) {
  int distance = abs(targetAngle - operationServoAngle);
  
  // Calculate dynamic step size based on the distance
  int dynamicStepSize = map(distance, 0, abs(right - left), 1, stepSize);
  dynamicStepSize = max(dynamicStepSize, 1); // Ensure step size is at least 1

  if (operationServoAngle < targetAngle) {
    operationServoAngle += dynamicStepSize;
    if (operationServoAngle > targetAngle) {
      operationServoAngle = targetAngle;
    }
  } else if (operationServoAngle > targetAngle) {
    operationServoAngle -= dynamicStepSize;
    if (operationServoAngle < targetAngle) {
      operationServoAngle = targetAngle;
    }
  }

  // Update the servo position in Operation Mode
  myServo.write(operationServoAngle);
}

// Function to blink LED a specified number of times without blocking
void blinkLEDNonBlocking(int times, unsigned long currentMillis) {
  static int blinkCount = 0;
  static bool isBlinking = false;
  static unsigned long lastBlinkTime = 0;

  if (!isBlinking) {
    blinkCount = times * 2; // Two toggles per blink
    isBlinking = true;
    lastBlinkTime = currentMillis;
  }

  if (isBlinking && (currentMillis - lastBlinkTime >= 200)) {
    toggleLED();
    lastBlinkTime = currentMillis;
    blinkCount--;

    if (blinkCount == 0) {
      isBlinking = false;
      analogWrite(ledPin, 255); // Keep LED on after blinking
    }
  }
}

// Function to toggle the LED state
void toggleLED() {
  ledState = !ledState; // Toggle the LED state

  if (ledState) {
    analogWrite(ledPin, 255); // Turn LED on at full brightness
  } else {
    analogWrite(ledPin, 0);   // Turn LED off
  }
}