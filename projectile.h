#ifndef PORJECTILE__H
#define PORJECTILE__H
#include <string>
#include <vector>
#include "sprite.h"


class ExplodingSprite;

class Projectile : public Sprite{
public:
  Projectile( const std::string& name);
  Projectile( const std::string& name, const bool isRight);
  Projectile(const Projectile& p);
  virtual ~Projectile();
  
  Projectile& operator=(const Projectile& rhs);
  
  void draw() const;
  void update(Uint32 ticks);
  void explode();
  
  bool getInUse() const { return inUse; }
  void setInUse(const bool flag) { inUse = flag; }
  
  bool getIsRight() const { return isRight; }
  void setIsRight(const bool flag) { isRight = flag; }
  
  int getDistanceCovered() const { return isRight; }
  void setDistanceCovered(const int distance) { 
    distanceCovered = distance;
    if(distanceCovered > lifeDistance){
      inUse = false;
    }
  }
  
  int getLifeDistance() const { return lifeDistance; }

private:
  ExplodingSprite* explosion;
  Sprite* explodeFrame;
	bool inUse;
  bool isRight;
  int distanceCovered;
	int lifeDistance; 
};
#endif
