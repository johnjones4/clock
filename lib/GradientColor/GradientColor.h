#ifndef GradientColor_H
#define GradientColor_H

#include <FastLED.h>

class GradientColor
{
public:
  GradientColor(float *hues, int nHues, float saturation , float lightness);
  void getColor(float pcnt, CRGB* rgb);
private:
  float *hues;
  int nHues;
  float saturation;
  float lightness;
};

#endif