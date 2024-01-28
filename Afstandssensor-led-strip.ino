#include <Adafruit_NeoPixel.h>

#define trigPin 10
#define echoPin 9
#define LED_PIN 6      // Pin voor de WS2812B LED-strip
#define NUMPIXELS 450   // Aantal LEDs op je strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

long duration;
int distance;
uint32_t currentColor = 0;  // Huidige kleur van de LED-strip

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  // Voeg hier je LED-strip aansturing toe op basis van de gemeten afstand
  uint32_t targetColor;

  if (distance < 65) {
    targetColor = strip.Color(255, 0, 0);  // Rood
  } else if (distance < 125) {
    targetColor = strip.Color(0, 255, 0);  // Groen
  } else if (distance < 175) {
    targetColor = strip.Color(0, 0, 255);  // Blauw
  } else {
    // Regenboog effect bij een afstand van 175 centimeter of meer
    rainbowEffect();
    delay(500); // Voeg een vertraging van 0,5 seconde toe nadat het regenboogeffect is voltooid
    return;
  }
  }

  // Geleidelijke kleurovergang
  colorTransition(currentColor, targetColor, 10);

  // Update de huidige kleur
  currentColor = targetColor;
}

// Functie om een geleidelijke kleurovergang te maken
void colorTransition(uint32_t startColor, uint32_t endColor, int steps) {
  for (int i = 0; i < steps; i++) {
    uint8_t r = map(i, 0, steps - 1, (startColor >> 16) & 0xFF, (endColor >> 16) & 0xFF);
    uint8_t g = map(i, 0, steps - 1, (startColor >> 8) & 0xFF, (endColor >> 8) & 0xFF);
    uint8_t b = map(i, 0, steps - 1, startColor & 0xFF, endColor & 0xFF);

    uint32_t interpolatedColor = strip.Color(r, g, b);

    colorWipe(interpolatedColor, 10);
  }
}

// Functie om de LED-strip op één kleur te laten oplichten
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(wait);
}

// Functie voor het regenboogeffect
void rainbowEffect() {
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < strip.numPixels(); j++) {
      strip.setPixelColor(j, Wheel((i + j) & 255));
    }
    strip.show();
    delay(3);
  }
}

// Functie voor het genereren van kleuren in het regenboogeffect
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
