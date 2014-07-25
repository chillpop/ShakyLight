#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
#include <avr/power.h>
#endif

#define PIXEL_PIN 0
#define ACCEL_PIN 1

#define SAMPLES 10
#define NUM_PIXELS 7
#define BRIGHTNESS 128

#define THRESHOLD 850

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

//start with red
int currentHue = 85;
//int desiredHue = 0;
//unsigned long lastTime = 0;

unsigned long upsideDownStartTime = 0;
#define UPSIDE_DOWN_THRESHOLD 

int loopsAboveThreshold = 0;
#define LOOP_THRESHOLD 5

void setup() {
  pixels.begin();
  pixels.show();

  setStripHue(currentHue);
  pixels.setBrightness(BRIGHTNESS);
}

void loop() {
  int accel = readAccelerometer();

  accel = abs(accel);

  if (accel > THRESHOLD) {
    loopsAboveThreshold++;
    if (loopsAboveThreshold > LOOP_THRESHOLD) {
      loopsAboveThreshold = 0;

      currentHue = currentHue + 1;
      if (currentHue > 255) {
        currentHue -= 255; 
      }
      setStripHue(currentHue);
      pixels.setBrightness(BRIGHTNESS);
    }
  }
}

int readAccelerometer() {
  long reading = 0;
  analogRead(ACCEL_PIN);
  delay(1);
  for (int i = 0; i < SAMPLES; i++)
  {
    reading += analogRead(ACCEL_PIN);
  }
  return reading/SAMPLES;
}

void setStripHue(byte hue) {
  for (int i = 0; i < NUM_PIXELS; i+=3) {
    pixels.setPixelColor(i, Wheel(hue));
  } 
  pixels.show();
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



