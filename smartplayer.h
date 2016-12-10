#ifndef SMARTPLAYER__H
#define SMARTPLAYER__H
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "player.h"
#include "vector2f.h"

class SmartPlayer : public Player {
public:
  SmartPlayer(const std::string& name1, const bool AIMode);
  ~SmartPlayer();
  
  void update(Uint32 ticks);
  void draw() const;
  void handleKeyStroke(const Uint8 *);
  void setUserPosition(Vector2f userPos){
    userPosition = userPos;
  }
  
private:
  bool AIMode;
  Vector2f userPosition;
  
};
#endif