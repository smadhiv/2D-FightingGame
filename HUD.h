#ifndef HUD__H
#define HUD__H
#include <stdint.h>
#include "aaline.h"
#include "subjectplayer.h"

class HUD {
public:
  HUD(float x0, float x1, float x2, float x3, float y0, float y1, float y2, float y3, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float thick);
  void draw(const Player *player1) const;
  
private:
  float x0, x1, x2, x3;
  float y0, y1, y2, y3;
  uint8_t r, g, b, a;
  float thick;
};
#endif
