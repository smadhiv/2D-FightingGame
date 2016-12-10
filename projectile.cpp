#include "projectile.h"
#include "explodingSprite.h"

Projectile::Projectile( const std::string& name):Sprite(name), explosion(NULL), explodeFrame(NULL), inUse(false), isRight(true), distanceCovered(0), lifeDistance(600) {}
Projectile::Projectile( const std::string& name, const bool isRight):Sprite(name), explosion(NULL), explodeFrame(NULL), inUse(false), isRight(isRight), distanceCovered(0), lifeDistance(600) {}
Projectile::Projectile(const Projectile& p):Sprite(p), explosion(NULL), explodeFrame(NULL), inUse(false), isRight(true), distanceCovered(0), lifeDistance(600) {}

Projectile::~Projectile() {
   if(explodeFrame){
    delete explodeFrame;
  }
  if( explosion ){ 
    delete explosion;
  }
}

Projectile& Projectile::operator=(const Projectile& rhs){
  if(explodeFrame){
    delete explodeFrame;
    explodeFrame = NULL;
  }
  if(explosion){
    delete explosion;
    explosion = NULL;
  }
  *this = rhs;
  return *this;
}

void Projectile::draw() const {
  if(explosion){
    explosion->draw();
    return;
  }
  if(inUse){
    Sprite::draw(); 
  }
}

void Projectile::update(Uint32 ticks) {
  if(explosion){
    inUse = false;
    explosion->update(ticks);
    if(explosion->chunkCount() == 0){
      delete explosion;
      explosion = NULL;
      delete explodeFrame;
      explodeFrame = NULL;
    }
    return;
  }
  
  if(inUse){
    if(isRight){
      X(X() + 40);
    }
    else{
      X(X() - 40);
    }
    distanceCovered += 40;
    if(distanceCovered > lifeDistance){
      inUse = false;
    }
  }
}

void Projectile::explode(){
  if(explodeFrame) return;
  explodeFrame = new Sprite(getName(), getPosition(), getVelocity(), getFrame());
  if(explosion){
    delete explosion;
    explosion = NULL;
  }
  explosion = new ExplodingSprite(*explodeFrame);
}