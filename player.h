#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "sprite.h"
#include "twowaysprite.h"
#include "collisionStrategy.h"
#include "gamedata.h"
#include "projectile.h"
#include "sound.h"

class ExplodingSprite;

class Player : public TwoWaySprite{
public:
  Player(const std::string&, const bool);
  Player(const Player&);
  virtual ~Player();
  Player& operator=(const Player& rhs);
  
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void explode();
  virtual void handleKeyStroke(const Uint8 *);
  
  bool checkForCollisions(Drawable* );
  void AvoidCollision(int objectCurrentFrame);

  void createProjectile();
  void drawProjectiles()const;
  void reset();
  void drawObjectPoolList()const;

 virtual int getActiveProjectiles()const {
   int count = 0;
   for(int i = 0; i < totalProjectiles; i++){
     if(projectiles[i]->getInUse()){
       count++;
     }
   }
	return count;
  }
  
  virtual int getAvailableProjectiles() const{
    return totalProjectiles - getActiveProjectiles();
  }


protected:
  ExplodingSprite* explosion;
  Sprite* explodeFrame;
  const bool isPlayer1;
  bool isMovement;
  bool isRight;
  bool isWalk;
  bool isDefend;
  bool isCrouch;
  bool isJump;
  bool isPunch;
  bool godMode;
  GameSound *sound;
  const int totalProjectiles;
  std::vector<CollisionStrategy* > strategies;
  std::vector<Projectile*> projectiles;
  int healthLevel;
  void advanceFrame();
  void setMovement();
  void handlePunch(const Drawable* obj1);
  void drawHealthBar() const;
  void drawEndScreen() const;
  void projectileHit(){
    (*sound)[2];
    if(godMode) return;
    healthLevel -= 5;
    if(healthLevel <= 0){
      explode();
      (*sound)[3];
    }    
  }
  void playerHit(){
    (*sound)[1];
    if(godMode) return;
    healthLevel -= 2;
    if(healthLevel <= 0){
      explode();
      (*sound)[3];
    }
  }
};
#endif
