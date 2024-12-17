#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <credentials.h>
#include <Clock.h>
#include <Face.h>
#include <GradientColor.h>

#define OUTER_RING_PIN  20
#define INNER_RING_PIN  8
#define LED_TYPE        WS2811
#define COLOR_ORDER     GRB
#define BRIGHTNESS      255
#define OUTER_OFFSET    0
#define INNER_OFFSET    0

// #define TEST_MODE
#define NO_LIGHTS

Clock clock1(UTC_OFFSET, USE_DST);

#ifndef NO_LIGHTS
float hues[3] = {0.65,0.12,0};
GradientColor c(hues, 3, 1, 0.5);
Face outer(60, 2);
Face inner(24, 1);
#endif

void setup() {
  Serial.begin(115200);

  Serial.println("Starting up");

#ifndef NO_LIGHTS

  outer.begin();
  inner.begin();

  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, OUTER_RING_PIN, COLOR_ORDER>(outer.leds, outer.max).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, INNER_RING_PIN, COLOR_ORDER>(inner.leds, inner.max).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  CRGB color = CRGB::White;
  outer.setHand(0, handpos{.index = OUTER_OFFSET, .color = color});
  inner.setHand(0, handpos{.index = INNER_OFFSET, .color = color});
  FastLED.show();
  FastLED.delay(1000);
#endif

#ifndef TEST_MODE
  Serial.printf("Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  clock1.begin();
#endif
  Serial.println("Ready");
}

void loop() {
#ifndef TEST_MODE
  struct tm timeinfo;
  clock1.update(&timeinfo);

#ifndef NO_LIGHTS
  float pct = (float)(timeinfo.tm_sec + (timeinfo.tm_min * 60) + (timeinfo.tm_hour * 60 * 60)) / 86400.0;

  CRGB color;
  c.getColor(pct, &color);

  int secs = (timeinfo.tm_sec + OUTER_OFFSET) % 60;
  int minutes = (timeinfo.tm_min + OUTER_OFFSET) % 60;
  int hours = ((timeinfo.tm_hour % 12 * 2) + INNER_OFFSET) % 24;

  outer.setHand(0, handpos{.index = secs,     .color = color});
  outer.setHand(1, handpos{.index = minutes,  .color = color});
  inner.setHand(0, handpos{.index = hours,    .color = color});

  FastLED.show();
  FastLED.delay(10);
#endif
#endif
}