#include <Adafruit_NeoPixel.h>
#include <Servo.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define NUMBER_OF_PIXELS 33
#define SERVO_MAX_DEGREES 180

Adafruit_NeoPixel A1LED = Adafruit_NeoPixel(NUMBER_OF_PIXELS, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel A2LED = Adafruit_NeoPixel(NUMBER_OF_PIXELS, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel A3LED = Adafruit_NeoPixel(NUMBER_OF_PIXELS, 7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel B1LED = Adafruit_NeoPixel(NUMBER_OF_PIXELS, 9, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel B2LED = Adafruit_NeoPixel(NUMBER_OF_PIXELS, 11, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel B3LED = Adafruit_NeoPixel(NUMBER_OF_PIXELS, 13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel C1LED = Adafruit_NeoPixel(NUMBER_OF_PIXELS, 15, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel C2LED = Adafruit_NeoPixel(NUMBER_OF_PIXELS, 17, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel C3LED = Adafruit_NeoPixel(NUMBER_OF_PIXELS, 19, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds[9] = {A1LED, A2LED, A3LED, B1LED, B2LED, B3LED, C1LED, C2LED, C3LED};

Servo A1SERVO;
Servo A2SERVO;
Servo A3SERVO;
Servo B1SERVO;
Servo B2SERVO;
Servo B3SERVO;
Servo C1SERVO;
Servo C2SERVO;
Servo C3SERVO;
Servo servos[9] = {A1SERVO, A2SERVO, A3SERVO, B1SERVO, B2SERVO, B3SERVO, C1SERVO, C2SERVO, C3SERVO};


// ----
 
void setup() {  
  Serial.begin(9600);
  
  A1SERVO.attach(2);
  A2SERVO.attach(4);
  A3SERVO.attach(6);
  B1SERVO.attach(8);
  B2SERVO.attach(10);  
  B3SERVO.attach(12);
  C1SERVO.attach(14);
  C2SERVO.attach(16);
  C3SERVO.attach(18);
 
  for (int i=0; i < 9; i++) {
    leds[i].begin();
    leds[i].show();
  }

//  setAllServosToPosition(90);
}

void loop() {
  rotateServoFromZero(50);
  rotateServoFromMaxDegrees(50);
}

// ----

void rotateServoFromZero(int timeToWaitInMs) {
  for (int i = 0; i <= SERVO_MAX_DEGREES; i++) {
    for (int j = 0; j<9; j++) {
      /** insert your favourite light behaviour during forward rotation here */
      lightTail(i, leds[j]);
      
      /** write Servo-position */
      servos[j].write(i);
    }
    
    delay(timeToWaitInMs);
  }
  fadeOut();
}

void rotateServoFromMaxDegrees(int timeToWaitInMs) {
  for (int i = SERVO_MAX_DEGREES; i >=0; i--) {
    for (int j = 0; j<9; j++) {
      /** insert your favourite light behaviour during backward rotation here */
      lightTailBack(i, leds[j]);
      
      /** write Servo-position */
      servos[j].write(i);
    }

    delay(timeToWaitInMs);
  }
  fadeOutBack();
}

// ----

/** 
 *  finds the right LED corresponding to servo's current position
 */
int calcPixel(int servoPos) {
  int result = map(servoPos, 0, SERVO_MAX_DEGREES, 0, NUMBER_OF_PIXELS);
  return result;
}

// ----

void lightForward(int i, Adafruit_NeoPixel &strip) {
    int t = calcPixel(i);
    int wc = 0;
    strip.setPixelColor(t, Wheel(wc, strip));
    int pxl = t-1;
    if (pxl < 0) pxl = strip.numPixels() - 1;
    strip.setPixelColor(pxl, strip.Color(0, 0, 0));
    strip.show();
    
    wc++;
    if (wc>255) wc=0;
}

void lightBackwards(int i, Adafruit_NeoPixel &strip) {
   int t = calcPixel(i);
   int wc = 0;
    strip.setPixelColor(t, Wheel(wc, strip));
    int pxl = t + 1;
    if (pxl >= strip.numPixels()) pxl = 0;
    strip.setPixelColor(pxl, strip.Color(0, 0, 0));
    strip.show();

    wc++;
    if (wc>255) wc=0;
}

// ----

void lightTail(int i, Adafruit_NeoPixel &strip) {
  int pixel = calcPixel(i); 
  
  uint32_t color = Wheel(i, strip);
  strip.setPixelColor(pixel, color);

  // calculate color for each tailPixel after "main" pixel
  for(int j = 1; j<9; j++) {
    int tailPixel = pixel-j;
    strip.setPixelColor(tailPixel, color);
  }
  // set first pixel after tail to BLACK
  strip.setPixelColor(pixel-9, strip.Color(0, 0, 0));
  strip.show();
}

void lightTailBack(int i, Adafruit_NeoPixel &strip) {  
  int pixel = calcPixel(i);
  
  uint32_t color = Wheel(i, strip);
  strip.setPixelColor(pixel, color);

// calculate color for each tailPixel after "main" pixel
  for(int j = 1; j<9; j++) {
    int tailPixel = pixel+j;
    strip.setPixelColor(tailPixel, color);
  }
  
  // set first pixel after tail to BLACK
  strip.setPixelColor(pixel+9, strip.Color(0, 0, 0));
  strip.show();
}

// Fades out all strips simultaneously when reaching to max position
void fadeOut() {
  for(int j = 9; j >= 0; j--) {
    for(int i = 0; i<=9; i++) {
      leds[i].setPixelColor(NUMBER_OF_PIXELS-j, leds[i].Color(0, 0, 0));
      leds[i].show();
    }
    // sorry, don't know why delay has to be set here - but without it the fadeout doesn't work.
    delay(50);
  }
}

// Fades out all strips simultaneously when returning to initial position
void fadeOutBack() {
  for(int j = 9; j >= 0; j--) {
    for(int i = 0; i<=9; i++) {
      leds[i].setPixelColor(j, leds[i].Color(0, 0, 0));
      leds[i].show();
    }
    // sorry, don't know why delay has to be set here - but without it the fadeout doesn't work.
    delay(50);
  }
}

// ----

void setAllServosToPosition(int degrees) {
  for (int i = 0; i <= degrees; i++) {
    for (int j = 0; j<9; j++) {
      servos[j].write(i);
    }
    delay(50);
  }
}

// ----

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel &strip) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
