#include <iostream>
#include <cmath>
#include <ctime>
#include "smartplayer.h"
#include "player.h"
#include "ioManager.h"

SmartPlayer::~SmartPlayer() {
}

SmartPlayer::SmartPlayer( const std::string& name1, const bool AIMode) :
  Player(name1, false), AIMode(AIMode), userPosition()
{ }

void SmartPlayer::update(Uint32 ticks) {
  if(AIMode){
    std::srand(std::time(0));
    if(healthLevel > 30){
      if(X() - userPosition[0] > 600){
        isWalk = true;
        isMovement = true;
        isRight = isDefend = isCrouch = isPunch = false;
      }
      else if(X() - userPosition[0] > 120 && X() - userPosition[0] <= 600){
        if(std::rand() % 3 == 0){
          isCrouch = false;
          isWalk = true;
          isMovement = true;
        }
        else if(std::rand() % 3 == 1){
           createProjectile(); 
           isCrouch = false;
           isWalk = false;
           isMovement = false;  
        }
        else{
          isCrouch = true;
          isMovement = true;
        }
        isRight = isDefend = isPunch = false;
      }
      else{
          if(std::rand() % 3 == 0){
            isPunch = true;
            isDefend = false;
            isCrouch = false;
            (*sound)[0];
          }
          else if(std::rand() % 3 == 1) {
            isDefend = true;
            isPunch = false;
            isCrouch = false;
          }
          else{
            isCrouch = true;
            isDefend = false;
            isPunch = false;
          }
        isMovement = true;
        isRight = isWalk = false;
      }
    }
    else{
      if(X() - userPosition[0] > 500 && X() - userPosition[0] < 600){
        isRight = isMovement = isPunch = isWalk = isDefend = isCrouch = false;
        if(std::rand() % 2 == 0){
          createProjectile();
        }
        else{
          isCrouch = true;
          isMovement = true;
        }
      }
      else if(X() - userPosition[0] >= 600){
        isWalk = true;
        isMovement = true;
        isRight = isDefend = isCrouch = isPunch = false;        
      }
      else{
        if( X() < worldWidth - 300){
          if(std::rand() % 2 == 0){
            isRight = true;
            isWalk = true;
            isMovement = true;
            isDefend = isCrouch = isPunch = false;
          }
          else{
            isRight = true;
            isCrouch = true;
            isMovement = true;
            isDefend = isWalk = isPunch = false;
          }
        }
        else{
          if(std::rand() % 3 == 0){
            (*sound)[0];
            isPunch = true;
            isDefend = false;
            isCrouch = false;
          }
          else if (std::rand() % 3 == 1){
            isDefend = true;
            isPunch = false;
            isCrouch = false;
          }
          else{
            isCrouch = true;
            isPunch = false;
            isDefend = true;
          }
          isMovement = true;
          isRight = isWalk = false;        
        }
      }
    }
  }  

  Player::update(ticks);
}

void SmartPlayer::draw() const { 
  Player::draw();
  if(AIMode){
    IOManager::getInstance().printMessageAt("You wish - AI Mode is on", worldWidth - 360, 75);
  }
}

void SmartPlayer::handleKeyStroke(const Uint8 *keystate) {
  if(keystate[SDLK_r]){
    AIMode = false;
  }
  if(keystate[SDLK_j]){
    AIMode = !AIMode;
  }
  if(!AIMode){
    Player::handleKeyStroke(keystate);
  }
}

 