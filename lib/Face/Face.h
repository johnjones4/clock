#ifndef Face_H
#define Face_H

#include <FastLED.h>

typedef struct {
  int index;
  CRGB color;
} handpos;

class Face
{
public:
  Face(int max, int nHands);
  void begin();
  void setHand(int hand, handpos pos);
  CRGB* leds;
  int max;
private:  
  int nHands;
  handpos* positions;
};

#endif