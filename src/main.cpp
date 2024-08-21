#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <credentials.h>
#include <Clock.h>
#include <Face.h>
#include <GradientColor.h>

#define OUTER_RING  14
#define INNER_RING  12
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS  255

Clock clock1(UTC_OFFSET, USE_DST);
float hues[3] = {0.65,0.12,0};
GradientColor c(hues, 3, 1, 0.5);
Face outer(60, 2);
Face inner(24, 1);

void setup() {
  Serial.begin(9600);
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
  outer.begin();
  inner.begin();

  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, OUTER_RING, COLOR_ORDER>(outer.leds, outer.max).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, INNER_RING, COLOR_ORDER>(inner.leds, inner.max).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
}

void loop() {
  struct tm timeinfo;
  clock1.update(&timeinfo);

  float pct = (float)(timeinfo.tm_sec + (timeinfo.tm_min * 60) + (timeinfo.tm_hour * 60 * 60)) / 86400.0;

  CRGB color;
  c.getColor(pct, &color);

  outer.setHand(0, handpos{.index = timeinfo.tm_sec,            .color = color});
  outer.setHand(1, handpos{.index = timeinfo.tm_min,            .color = color});
  inner.setHand(0, handpos{.index = timeinfo.tm_hour % 12 * 2,  .color = color});

  FastLED.show();
  FastLED.delay(10);
}