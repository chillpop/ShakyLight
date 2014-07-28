#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
#include <avr/power.h>
#endif

#define PIXEL_PIN 0
#define ACCEL_PIN 1

//average the accelerometer over this many readings to reduce noise
#define ACCEL_SAMPLES 10
#define NUM_PIXELS 8
//using an 8 pixels board for space, but not all of the pixels for power savings
#define NUM_ACTIVE_PIXELS 3
const int activePixels[] = {0, 3, 7};

#define SHAKE_THRESHOLD 256
#define SHAKE_COUNT_THRESHOLD 3
//in order to change color, you have to shake SHAKE_COUNT_THRESHOLD times
int shakeCount = 0;

//start with red
int currentHue = 85;
#define HUE_INCREMENT 1

#define BRIGHTNESS_INCREMENT 10
#define BRIGHTNESS_DECREMENT 0.01
#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 255
//start at min brightness
float currentBrightness = MIN_BRIGHTNESS;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

unsigned long upsideDownStartTime = 0;
#define UPSIDE_DOWN_TIME_THRESHOLD 250
//if we're upside down for more than this time, do something

void setup() {
  pixels.begin();
  setStripHue(currentHue);
  pixels.setBrightness(currentBrightness);
  pixels.show();
}

void loop() {
  int accel = readAccelerometer();

  if (abs(accel) > SHAKE_THRESHOLD) {
    shakeCount++;
    if (shakeCount > SHAKE_COUNT_THRESHOLD) {
      shakeCount = 0;

      currentHue += HUE_INCREMENT;
      if (currentHue > 255) {
        currentHue -= 255;
      }
      
      //when we shake, make it brighter
      currentBrightness += BRIGHTNESS_INCREMENT; 
    }
  } 
  else {
    //when there's no shake, make it darker
    currentBrightness -= BRIGHTNESS_DECREMENT;
  }

  currentBrightness = constrain(currentBrightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);

  if (accel < 0) {
    unsigned long now = millis();
    if (upsideDownStartTime == 0) {
      upsideDownStartTime = now;
    } else if (now - upsideDownStartTime > UPSIDE_DOWN_TIME_THRESHOLD) {
      //when upside down, "hide" by turning off the lights 
      currentBrightness = 0;
    }
  } else {
    upsideDownStartTime = 0;
  }

  setStripHue(currentHue);
  pixels.setBrightness((byte)currentBrightness);
  pixels.show();
}

int readAccelerometer() {
  long reading = 0;
  analogRead(ACCEL_PIN);
  delay(1);
  for (int i = 0; i < ACCEL_SAMPLES; i++)
  {
    reading += analogRead(ACCEL_PIN);
  }
  int accel = reading/ACCEL_SAMPLES;
  //accel is between 0 and 1023
  //512 is (approx) the zero point (no shaking)
  accel -= 512;
  
  return accel;
}

void setStripHue(byte hue) {
  for (int i = 0; i < NUM_ACTIVE_PIXELS; i++) {
    int p = activePixels[i];
    pixels.setPixelColor(p, Wheel(hue));
  } 
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}




