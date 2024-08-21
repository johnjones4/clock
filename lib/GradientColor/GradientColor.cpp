#include <GradientColor.h>

// Helper function to convert hue to RGB
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

float computeRealPercentage(float nHues, float pcnt, int *idx) {
  float rangeSize = 1.0 / (float)nHues;
  int idx1 = (int)floor(pcnt / rangeSize);
  *idx = idx1;
  float base = (float)idx1 * rangeSize;
  return (pcnt - base) / rangeSize;
}

float computeValue(float a, float b, float percent) {
    float result;

    // Calculate the distance between a and b, considering wrapping around from 1 to 0
    if (b >= a) {
        result = a + percent * (b - a);
    } else {
        result = a + percent * ((1.0f - a) + b);
        if (result > 1.0f) {
            result -= 1.0f; // Wrap around from 1 back to 0
        }
    }

    return result;
}

GradientColor::GradientColor(float *hues, int nHues, float saturation , float lightness)
{
  this->hues = hues;
  this->nHues = nHues;
  this->saturation = saturation;
  this->lightness = lightness;
};

void GradientColor::getColor(float pcnt, CRGB* rgb)
{
  int idx;
  float truePcnt = computeRealPercentage(nHues, pcnt, &idx);
  int a = hues[idx % nHues];
  int b = hues[(idx + 1) % nHues];
  float hue = computeValue(a, b, truePcnt);
  hsl_to_rgb(hue, saturation, lightness, rgb);
};