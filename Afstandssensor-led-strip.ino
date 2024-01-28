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
  } else {
    targetColor = strip.Color(0, 0, 255);  // blauw
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
