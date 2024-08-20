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
    this->positions[i].length = 0;
    this->positions[i].color = 0;
  }
  for (int i = 0; i < max; i++) {
    this->leds[i] = 0;
  }
}

void Face::setHand(int hand, int index, int length, CRGB color)
{
  if (hand >= nHands) {
    return;
  }
  if (index >= max) {
    return;
  }

  if (index == this->positions[hand].index && length == this->positions[hand].length && color == this->positions[hand].color) {
    return;
  }

  bool needsClear = true;
  for (int i = 0; i < nHands; i++) {
    if (i != hand && this->positions[i].index == index) {
      int length1 = length > this->positions[i].length ? length : this->positions[i].length;
      needsClear = false;
      for (int j = index; j < index + length1; j++) {
        if (j < max) {
          this->leds[j] = (j - index) >= this->positions[i].length ? 0 : this->positions[i].color;
        }
      }
    }
  }

  if (needsClear) {
    for (int j = this->positions[hand].index; j < this->positions[hand].index + this->positions[hand].length; j++) {
      this->leds[j] = 0;
    }
  }

  this->positions[hand].index = index;
  this->positions[hand].length = length;
  this->positions[hand].color = color;

  for (int j = this->positions[hand].index; j < this->positions[hand].index + this->positions[hand].length; j++) {
    this->leds[j] = this->positions[hand].color;
  }
}