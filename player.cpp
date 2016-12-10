#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"
#include <cmath>
#include "ioManager.h"
#include "aaline.h"

Player::Player( const std::string& name, const bool isPlayer1) :
  TwoWaySprite(name), explosion(NULL), explodeFrame(NULL), isPlayer1(isPlayer1), isMovement(false), isRight(true), isWalk(false), isDefend(false), isCrouch(false), isJump(false), isPunch(false), godMode(false), sound(new GameSound()), totalProjectiles(5),strategies(), projectiles(), healthLevel(100) { 
  strategies.push_back( new MidPointCollisionStrategy );
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  projectiles.reserve(totalProjectiles);
  for(int i = 0; i < totalProjectiles; i++){
    projectiles.push_back(new Projectile("projectiles"));
  }
  SDL_EnableKeyRepeat(frameInterval, SDL_DEFAULT_REPEAT_INTERVAL);
}

Player::Player(const Player& s) : TwoWaySprite(s), explosion(NULL), explodeFrame(NULL), isPlayer1(s.isPlayer1), isMovement(false), isRight(true), isWalk(false), isDefend(false), isCrouch(false), isJump(false), isPunch(false), godMode(false), sound(new GameSound()), totalProjectiles(5),  strategies(), projectiles(), healthLevel(100) { }

Player::~Player(){ 
  if(explodeFrame){
    delete explodeFrame;
  }
  if( explosion ){ 
    delete explosion;
  }
  for (unsigned i = 0; i < projectiles.size(); ++i) {
    delete projectiles[i];
  }
  projectiles.clear();
  
  for (unsigned i = 0; i < strategies.size(); ++i) {
    delete strategies[i];
  }
  strategies.clear();
  delete sound;
}

Player& Player::operator=(const Player& rhs){
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

void Player::draw() const {
  
  drawHealthBar();
  
  if(healthLevel <= 0){
    if(explosion){
      explosion->draw();
    }
    drawEndScreen();
    return;
  }
  drawProjectiles();
  TwoWaySprite::draw();
}

void Player::update(Uint32 ticks) {
  
  if(healthLevel <= 0){
    if(explosion){
      explosion->update(ticks);
      if(explosion->chunkCount() == 0){
        delete explosion;
        explosion = NULL;
        delete explodeFrame;
        explodeFrame = NULL;
        X(worldWidth + 100);
      }
    }
    return;
  }
  
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame < frameInterval){
    return;
  }

  for( int i = 0; i < totalProjectiles; i++ ) {
    projectiles[i]->update(ticks);
  }
  
  advanceFrame();
  if(isMovement){
    if(isWalk){
      if(!isRight){
        if(X() > -70){
          X(X() - 40);
        }
      }
      else{
        if( X() < worldWidth - 245){
          X(X() + 40);  
        }
      }
    }
  }
}

void Player::explode(){
  if(explodeFrame) return;
  explodeFrame = new Sprite(getName(), getPosition(), getVelocity(), getFrame());
  if(explosion){
    delete explosion;
    explosion = NULL;
  }
  explosion = new ExplodingSprite(*explodeFrame);
}

void Player::handleKeyStroke(const Uint8 *keystate) {
  int count = 0;
  bool prevIsRight = isRight;
  if(isPlayer1){
    if(keystate[SDLK_a]){
      isMovement = true;
      isRight = false;
      isWalk = true;
      isCrouch = isDefend = isPunch = false;
      count++;
    }
    if(keystate[SDLK_d]){
      isMovement = true;
      isRight = true;
      isWalk = true;
      isCrouch = isDefend = isPunch = false;
      count++;
    }
    if(keystate[SDLK_s]){
      isMovement = true;
      isCrouch = true;
      isWalk = isDefend = isPunch = false;
    }
    if(keystate[SDLK_w]){
      isMovement = true;
      isDefend = true;
      isWalk = isCrouch = isPunch = false;
    }
    if(keystate[SDLK_1]){
      (*sound)[0];
      isPunch = true;
      isMovement = true;
      isWalk = isCrouch = isDefend = false;
    }
    if(keystate[SDLK_2]){
		  createProjectile();
    }
    if(keystate[SDLK_g]){
		  godMode = !godMode;
    }
  }
  else{
    if(keystate[SDLK_LEFT]){
      isMovement = true;
      isRight = false;
      isWalk = true;
      isCrouch = isDefend = isPunch = false;
      count++;
    }
    if(keystate[SDLK_RIGHT]){
      isMovement = true;
      isRight = true;
      isWalk = true;
      isCrouch = isDefend = isPunch = false;
      count++;
    }
    if(keystate[SDLK_DOWN]){
      isMovement = true;
      isCrouch = true;
      isWalk = isDefend = isPunch = false;
    }
    if(keystate[SDLK_UP]){
      isMovement = true;
      isDefend = true;
      isWalk = isCrouch = isPunch = false;
    }
    if(keystate[SDLK_9]){
      (*sound)[0];
      isPunch = true;
      isMovement = true;
      isWalk = isCrouch = isDefend = false;
    }
    if(keystate[SDLK_0]){
		  createProjectile();
    }
    if(keystate[SDLK_h]){
		  godMode = !godMode;
    }
  }
  if(count > 1){
    isWalk = isCrouch = isDefend = isPunch = isMovement = false;
    isRight = prevIsRight;
  }
  
  if ( keystate[SDLK_r] ) {
    X(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"));
    Y(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y"));
    healthLevel = 100;
    godMode = false;
    if(isPlayer1){
      isRight = true;
    }
    else{
      isRight = false;
    }
  }
}

void Player::advanceFrame() {
  timeSinceLastFrame = 0;
  
  currentFrame = (currentFrame + 1) % 3;
  if (!isRight){
    currentFrame += 3;
  }
  
  if(isMovement){
    if(isWalk){
      currentFrame += 6;
      if(currentFrame == 8 || currentFrame == 11){
        isWalk = isMovement = false;
      }
    }
    else if(isDefend){
      currentFrame += 12;
      if(currentFrame == 14 || currentFrame == 17){
        isDefend = isMovement = false;
      }
    }
  
    else if(isCrouch){
      if(currentFrame < 6){
        currentFrame += 18;
      }
      if(currentFrame == 20 || currentFrame == 23){
        isCrouch = isMovement = false;
      }
    }

    else if(isPunch){
      if(currentFrame < 6){
        currentFrame += 24;
      }
      if(currentFrame == 26 || currentFrame == 29){
        isPunch = isMovement = false;
      }
    }
  }
}

bool Player::checkForCollisions(Drawable* obj1) {
  for(int i = 0; i < totalProjectiles; i++){
    if(projectiles[i]->getInUse()){
      if(strategies[2]->execute(*obj1, *(projectiles[i]))){
        projectiles[i]->explode();
        projectiles[i]->setInUse(false);
        (*obj1).projectileHit();
      }
    }
  }
  if(strategies[1]->execute(*obj1, *this)){
     if(strategies[2]->execute(*obj1, *this)){
       if( timeSinceLastFrame == 0 && healthLevel > 0) {
         handlePunch(obj1);
       }
       return true;
     }
  }
  return false;
}

void Player::AvoidCollision(int objectCurrentFrame) {
  if(isPlayer1){
    if(!isWalk || isPunch || objectCurrentFrame == 29){
      return;
    }
    if(X() > 0){
      X(X() - 5);
    }
  }
  else{
    if(!isWalk || isPunch || objectCurrentFrame == 26){
      return;
    }
    if(X() < worldWidth){
      X(X() + 5);
    }
  }
  timeSinceLastFrame = 0;
}

void Player::setMovement() {
  isMovement = false;
}

void Player::handlePunch(const Drawable* obj1) {
  if(obj1->getCurrentFrame() == 29){
    if(isPlayer1 && currentFrame != 26 && !((currentFrame >= 12 && currentFrame <= 14) || (currentFrame >= 18 && currentFrame <= 20))){
      playerHit();
    }
  }
  else if(obj1->getCurrentFrame() == 26){
    if(!isPlayer1 && currentFrame != 29 && !((currentFrame >= 15 && currentFrame <= 17) || (currentFrame >= 21 && currentFrame <= 23))){
      playerHit();
    }
  }
}

void Player::drawHealthBar() const {
  int healthBarHeight = 30;
  int healthBarWidth = 300;
  int x = 60;
  int y = 40;
  if(!isPlayer1){
    x = worldWidth - 360;
  }
  Draw_AALine(IOManager::getInstance().getScreen(), x - 1, y, x + healthBarWidth + 1, y, 0, 0, 0, 0, 200);
  Draw_AALine(IOManager::getInstance().getScreen(), x - 1, y + healthBarHeight, x + healthBarWidth + 1, y + healthBarHeight, 0, 0, 0, 0, 200);
  Draw_AALine(IOManager::getInstance().getScreen(), x - 1, y, x - 1, y + healthBarHeight, 0, 0, 0, 0, 200);
  Draw_AALine(IOManager::getInstance().getScreen(), x + healthBarWidth + 1, y, x + healthBarWidth + 1, y + healthBarHeight, 0, 0, 0, 0, 200);
  
  if(healthLevel >= 60){
    Draw_AALine(IOManager::getInstance().getScreen(), x, y + 15, x + (healthLevel * 300 / 100), y + 15, healthBarHeight - 2, 0, 255, 0, 200);
  }
  else if(healthLevel >= 30){
    Draw_AALine(IOManager::getInstance().getScreen(), x, y + 15, x + (healthLevel * 300 / 100), y + 15, healthBarHeight - 2, 255, 255, 0, 200);
  }
  else if(healthLevel >= 0){
    Draw_AALine(IOManager::getInstance().getScreen(), x, y + 15, x + (healthLevel * 300 / 100), y + 15, healthBarHeight - 2, 255, 0, 0, 200);
  }
  else{
    Draw_AALine(IOManager::getInstance().getScreen(), x, y + 15, x + 300, y + 15, healthBarHeight - 2, 10, 10, 10, 200);
  }
  
  if(godMode){
    IOManager::getInstance().printMessageAt("You CHEAT - God Mode is on", x, y + 35);
  }
}

void Player::drawEndScreen() const {
  int x = 420;
  int y = 200;
  int healthBarHeight = 100;
  int healthBarWidth = 300;
  Draw_AALine(IOManager::getInstance().getScreen(), x - 1, y, x + healthBarWidth + 1, y, 0, 0, 0, 0, 200);
  Draw_AALine(IOManager::getInstance().getScreen(), x - 1, y + healthBarHeight, x + healthBarWidth + 1, y + healthBarHeight, 0, 0, 0, 0, 200);
  Draw_AALine(IOManager::getInstance().getScreen(), x - 1, y, x - 1, y + healthBarHeight, 0, 0, 0, 0, 200);
  Draw_AALine(IOManager::getInstance().getScreen(), x + healthBarWidth + 1, y, x + healthBarWidth + 1, y + healthBarHeight, 0, 0, 0, 0, 200);
  Draw_AALine(IOManager::getInstance().getScreen(), x, y + healthBarHeight/2, x + healthBarWidth, y + healthBarHeight/2, healthBarHeight - 2, 255, 255, 255, 200);
  IOManager::getInstance().printMessageAt("Game Over!!", 510, 210);
  if(isPlayer1){
    IOManager::getInstance().printMessageAt("  You Lost", 510, 260);
  }
  else{
    IOManager::getInstance().printMessageAt("  You won", 510, 260);
  }
}

/////////////////////////////////////Projectiles Functions

void Player::createProjectile() {
	for( int i = 0; i < totalProjectiles; i++ ){
	  if(!(projectiles[i]->getInUse())){
      (*sound)[1];
      projectiles[i]->setInUse(true);
      projectiles[i]->setDistanceCovered(0);
	    if(isRight){
        projectiles[i]->setIsRight(true);
        projectiles[i]->setPosition(getPosition() + Vector2f(150,80));
      }
      else{
        projectiles[i]->setIsRight(false);
        projectiles[i]->setPosition(getPosition() + Vector2f(80,70));
      }
      return;
	  }
  }
}

void Player::drawProjectiles() const {
	for( int i = 0; i< totalProjectiles; i++){ 
      projectiles[i]->draw();
	}
}



