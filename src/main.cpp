#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <credentials.h>
#include <Clock.h>
#include <Face.h>

#define OUTER_RING  14
#define INNER_RING  12
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS  255

#define SECONDS_IN_MINUTE 60.0
#define SECONDS_IN_HOUR (60.0 * 60.0)
#define SECONDS_IN_DAY (60.0 * 60.0 * 24.0)

Clock clock1(UTC_OFFSET, USE_DST);
Face outer(60, 2);
Face inner(24, 1);

int secondStep = 0;
int minuteStep = 0;
int hourStep = 0;

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

float hue_to_rgb(float p, float q, float t) {
    if (t < 0.0f) t += 1.0f;
    if (t > 1.0f) t -= 1.0f;
    if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
    if (t < 1.0f / 2.0f) return q;
    if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    return p;
}

// Function to convert HSL to RGB
void hsl_to_rgb(float h, float s, float l, CRGB* rgb) {
    float q, p;

    if (s == 0.0f) {
        // Achromatic (grey)
        rgb->r = rgb->g = rgb->b = (uint8_t)(l * 255.0f);
    } else {
        if (l < 0.5f) {
            q = l * (1.0f + s);
        } else {
            q = l + s - l * s;
        }
        p = 2.0f * l - q;

        float red = hue_to_rgb(p, q, h + 1.0f / 3.0f);
        float green = hue_to_rgb(p, q, h);
        float blue = hue_to_rgb(p, q, h - 1.0f / 3.0f);

        // Convert from float [0, 1] to uint8_t [0, 255]
        rgb->r = (uint8_t)(red * 255.0f);
        rgb->g = (uint8_t)(green * 255.0f);
        rgb->b = (uint8_t)(blue * 255.0f);
    }
}

void loop() {
  struct tm timeinfo;
  clock1.update(&timeinfo);

  float secondPct = (float)((timeinfo.tm_sec + secondStep) % 60) / SECONDS_IN_MINUTE;
  float minutePct = (float)(timeinfo.tm_sec + (((timeinfo.tm_min + minuteStep) % 60) * 60)) / SECONDS_IN_HOUR;
  float hourPct = (float)(timeinfo.tm_sec + (timeinfo.tm_min * 60) + (((timeinfo.tm_hour + hourStep) % 24) * 60 * 60)) / SECONDS_IN_DAY;

  if (secondPct == 0) {
    secondStep++;
  }
  if (minuteStep == 0) {
    minuteStep++;
  }
  if (hourStep == 0) {
    hourStep++;
  }

  CRGB second;
  CRGB minute;
  CRGB hour;
  hsl_to_rgb(secondPct, 1, 0.5, &second);
  hsl_to_rgb(minutePct, 1, 0.5, &minute);
  hsl_to_rgb(hourPct, 1, 0.5, &hour);

  outer.setHand(0, handpos{.index = timeinfo.tm_sec,            .color = second});
  outer.setHand(1, handpos{.index = timeinfo.tm_min,            .color = minute});
  inner.setHand(0, handpos{.index = timeinfo.tm_hour % 12 * 2,  .color = hour});

  FastLED.show();
  FastLED.delay(10);
}