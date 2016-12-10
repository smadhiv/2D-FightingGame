#include <iostream>
#include <cmath>
#include "subjectplayer.h"

SubjectPlayer::~SubjectPlayer() {
  delete player2;
}

SubjectPlayer::SubjectPlayer( const std::string& name1, const std::string& name2) :
  Player(name1, true),
  player2(new SmartPlayer(name2, false))
{ }

void SubjectPlayer::update(Uint32 ticks) { 
  Player::update(ticks);
  if(checkForCollisions(player2)){
    AvoidCollision(player2->getCurrentFrame());
  }
  
  static_cast<SmartPlayer* >(player2)->setUserPosition(Player::getPosition());
  player2->update(ticks);
  if(player2->checkForCollisions(this)){
    player2->AvoidCollision(getCurrentFrame());
  }
}

void SubjectPlayer::draw() const { 
  Player::draw();
  player2->draw();
}

void SubjectPlayer::handleKeyStroke(const Uint8 *keystate) {
  Player::handleKeyStroke(keystate);
  player2->handleKeyStroke(keystate);
}
  
  
  

