#include <Face.h>
#include <Arduino.h>

Face::Face(int max, int nHands)
{
  this->max = max;
  this->nHands = nHands;
}

void Face::begin() 
{
  this->positions = (handpos*)malloc(sizeof(handpos) * this->nHands);
  this->leds = (CRGB*)malloc(sizeof(CRGB) * this->max);
  for (int i = 0; i < nHands; i++) {
    this->positions[i].index = 0;
    this->positions[i].color = 0;
  }
  for (int i = 0; i < max; i++) {
    this->leds[i] = 0;
  }
}

void Face::setHand(int hand, handpos pos)
{
  if (hand >= nHands) {
    return;
  }
  if (pos.index >= max) {
    return;
  }

  if (pos.index == this->positions[hand].index && pos.color == this->positions[hand].color) {
    return;
  }

  bool needsClear = true;
  for (int i = 0; i < nHands; i++) {
    if (i != hand && this->positions[i].index == this->positions[hand].index) {
      needsClear = false;
      this->leds[this->positions[i].index] = this->positions[i].color;
    }
  }

  if (needsClear) {
    this->leds[this->positions[hand].index] = 0;
  }

  this->positions[hand] = pos;

  this->leds[this->positions[hand].index] = this->positions[hand].color;
}