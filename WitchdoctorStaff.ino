/* 

         )  (                       
   (  ( /(  )\ )       (            
   )\ )\())(()/(       )\   ) (     
 (((_|(_\)  /(_))`  ) ((_| /( )\ )  
 )\___ ((_)(_))  /(/(  _ )(_)|()/(  
((/ __/ _ \/ __|((_)_\| ((_)(_ )(_)) 
 | (_| (_) \__ \| '_ \) / _` | || | 
  \___\___/|___/| .__/|_\__,_|\_, | 
                |_| controller |__/  

COSplay LED modular controller

 hardware platform  : Adafruit Metro Mini v2
                      Pro Trinket Lipoly backpack
 LEDs               : LEDstrip Neopixel 60 LEDS/m - WS2812B
                      Flora Neopixel V2
 Power              : 3.7v - 1800 mAh LiPo or 3.7v - 4400 mAh LiPo
 Codebase           : Arduino (C/C++)

Switches-Buttons
- digital port 2    : button (<color> cable) - trigger
- digital port 3    : switch (blue cable) - switch pre-defined animations A1-A2-A1B1-A1B2-A2B1-A2B2 
- digital port 4    : switch (yellow cable) - switch pre-defined animations B1-B2-A1B1-A1B2-A2B1-A2B2

Connectos
 - digital port 6   : two connectors LED strip
 - digital port 7   : one connector LED strip or steady lights
 - digital port 8   : weapon trigger or other LED/sound function (aux) 

 (2024) JinjiroSan

 ledcosplay.ino : v1-1.3 (alpha) - refactor C1.0.0
*/

#include <Adafruit_NeoPixel.h>

#define LED_PIN_C6     6  // two connectors for LEDstrips
#define LED_PIN_C7     7  // one connector for LEDstrips

// How many NeoPixels are attached to each pin
#define LED_COUNT_C6  18
#define LED_COUNT_C7  30

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS_C6 50
#define BRIGHTNESS_C7 10

#define BUTTON_PIN_T2 2
#define BUTTON_PIN_T3 3
#define BUTTON_PIN_T4 4

// Brightness levels for each pattern
int brightnessPatternOne = 50;
int brightnessPatternTwo = 50;
int brightnessPatternThree = 75;
int brightnessPatternFour = 30;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip_C6(LED_COUNT_C6, LED_PIN_C6, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_C7(LED_COUNT_C7, LED_PIN_C7, NEO_GRBW + NEO_KHZ800);

int buttonState_T2 = 0;
int buttonState_T3 = 0;
int buttonState_T4 = 0;

void setup() {
  strip_C6.begin();
  strip_C6.setBrightness(BRIGHTNESS_C6);
  strip_C6.show();

  strip_C7.begin();
  strip_C7.setBrightness(BRIGHTNESS_C7);
  strip_C7.show();

  pinMode(BUTTON_PIN_T2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_T3, INPUT_PULLUP);
  pinMode(BUTTON_PIN_T4, INPUT_PULLUP);
}

void patternOne() {
  // Set brightness for both strips before starting the pattern
  strip_C6.setBrightness(brightnessPatternOne);
  strip_C7.setBrightness(brightnessPatternOne);
  strip_C6.show();
  strip_C7.show();
  
  // Set alternating colors (blue-purple) for both strips
  for (int i = 0; i < strip_C6.numPixels(); i++) {
    if (i % 2 == 0) {
      strip_C6.setPixelColor(i, strip_C6.Color(0, 0, 255));  // Blue
    } else {
      strip_C6.setPixelColor(i, strip_C6.Color(128, 0, 128));  // Purple
    }
  }
  for (int i = 0; i < strip_C7.numPixels(); i++) {
    if (i % 2 == 0) {
      strip_C7.setPixelColor(i, strip_C7.Color(0, 0, 255));  // Blue
    } else {
      strip_C7.setPixelColor(i, strip_C7.Color(128, 0, 128));  // Purple
    }
  }
  strip_C6.show();
  strip_C7.show();

  // Breathing effect using a sine wave pattern for smooth brightness changes
  for (int i = 0; i < 360; i += 5) {
    float radian = i * (3.14159 / 180.0);  // Convert degrees to radians
    int brightness = (sin(radian) * 127) + 128;  // Calculate brightness (0-255)

    int adjustedBrightness = (brightness * brightnessPatternOne) / 255; // Adjust brightness according to patternOne level

    strip_C6.setBrightness(adjustedBrightness);
    strip_C7.setBrightness(adjustedBrightness);
    strip_C6.show();
    strip_C7.show();
    delay(30);
  }

  // Reset brightness to original value
  strip_C6.setBrightness(BRIGHTNESS_C6);
  strip_C7.setBrightness(BRIGHTNESS_C7);
  strip_C6.show();
  strip_C7.show();
}

void patternTwo() {
  strip_C6.setBrightness(brightnessPatternTwo);
  strip_C7.setBrightness(brightnessPatternTwo);
  strip_C6.show();
  strip_C7.show();

  // Step 1: Animate C7 strip in red from end to start
  for (int i = strip_C7.numPixels() - 1; i >= 0; i--) {
    strip_C7.setPixelColor(i, strip_C7.Color(255, 0, 0));
    strip_C7.show();
    delay(50);
    if (i <= strip_C7.numPixels() - 4) {
      strip_C7.setPixelColor(i + 3, strip_C7.Color(0, 0, 0));
    }
  }
  for (int i = 0; i < 3; i++) {
    strip_C7.setPixelColor(i, strip_C7.Color(0, 0, 0));
    strip_C7.show();
    delay(50);
  }

  // Step 2: Animate C6 strip in red from end to start (Pattern One)
  for (int i = strip_C6.numPixels() - 1; i >= 0; i--) {
    strip_C6.setPixelColor(i, strip_C6.Color(180, 0, 0));
    strip_C6.show();
    delay(50);
  }

  // Reset brightness to original value
  strip_C6.setBrightness(BRIGHTNESS_C6);
  strip_C7.setBrightness(BRIGHTNESS_C7);
  strip_C6.show();
  strip_C7.show();
}

void patternThree() {
  strip_C6.setBrightness(brightnessPatternThree);
  strip_C7.setBrightness(brightnessPatternThree);
  strip_C6.show();
  strip_C7.show();

  for (int i = 0; i < 50; i++) {
    int randomPixel_C6 = random(strip_C6.numPixels());
    int randomPixel_C7 = random(strip_C7.numPixels());
    
    strip_C6.setPixelColor(randomPixel_C6, strip_C6.Color(0, 0, 0, 255));
    strip_C7.setPixelColor(randomPixel_C7, strip_C7.Color(0, 0, 0, 255));
    
    strip_C6.show();
    strip_C7.show();
    
    delay(50);
    
    strip_C6.setPixelColor(randomPixel_C6, strip_C6.Color(0, 0, 0, 0));
    strip_C7.setPixelColor(randomPixel_C7, strip_C7.Color(0, 0, 0, 0));
  }

  // Reset brightness to original value
  strip_C6.setBrightness(BRIGHTNESS_C6);
  strip_C7.setBrightness(BRIGHTNESS_C7);
  strip_C6.show();
  strip_C7.show();
}

void patternFour() {
  strip_C6.setBrightness(brightnessPatternFour);
  strip_C7.setBrightness(brightnessPatternFour);
  strip_C6.show();
  strip_C7.show();

  unsigned long startTimeC6 = millis();
  unsigned long startTimeC7 = millis();
  int c6Index = strip_C6.numPixels() - 1;
  int c7Index = strip_C7.numPixels() - 1;
  bool c6Running = false;

  while (c6Index >= 0 || c7Index >= 0) {
    unsigned long currentTime = millis();

    // Update C7 strip independently
    if (currentTime - startTimeC7 >= 100 && c7Index >= 0) {
      strip_C7.setPixelColor(c7Index, strip_C7.Color(0, 100, 0));
      if (c7Index <= strip_C7.numPixels() - 4) {
        strip_C7.setPixelColor(c7Index + 3, strip_C7.Color(0, 0, 0));
      }
      strip_C7.show();
      c7Index--;
      startTimeC7 = currentTime;
    }

    // Start C6 sequence concurrently when C7 reaches the last 4 LEDs
    if (!c6Running && c7Index <= 3) {
      c6Running = true;
    }

    // Continue running C6 sequence independently
    if (c6Running && c6Index >= 0 && currentTime - startTimeC6 >= 100) {
      strip_C6.setPixelColor(c6Index, strip_C6.Color(0, 100, 0));
      if (c6Index <= strip_C6.numPixels() - 3) {
        strip_C6.setPixelColor(c6Index + 2, strip_C6.Color(0, 0, 0));
      }
      strip_C6.show();
      c6Index--;
      startTimeC6 = currentTime;
    }

    // Ensure C7 turns off the last LEDs properly
    if (c7Index < 0 && c6Running) {
      for (int i = 3; i >= 0; i--) {
        strip_C7.setPixelColor(i, strip_C7.Color(0, 0, 0));
      }
      strip_C7.show();
      c6Running = false;
    }
  }

  // Reset brightness to original value
  strip_C6.setBrightness(BRIGHTNESS_C6);
  strip_C7.setBrightness(BRIGHTNESS_C7);
  strip_C6.show();
  strip_C7.show();
}

void loop() {
  strip_C6.clear();
  strip_C6.show();
  strip_C7.clear();
  strip_C7.show();

  buttonState_T3 = digitalRead(BUTTON_PIN_T3);
  buttonState_T4 = digitalRead(BUTTON_PIN_T4);

  if (buttonState_T3 == LOW && buttonState_T4 == LOW) {
    patternOne();
  } else if (buttonState_T3 == LOW && buttonState_T4 == HIGH) {
    patternTwo();
  } else if (buttonState_T3 == HIGH && buttonState_T4 == LOW) {
    patternThree();
  } else if (buttonState_T3 == HIGH && buttonState_T4 == HIGH) {
    patternFour();
  }
}
