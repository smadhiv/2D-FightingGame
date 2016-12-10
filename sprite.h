#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"


class ExplodingSprite;
class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const std::string&, const Vector2f& pos, const Vector2f& vel);
  Sprite(const std::string&, 
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  Sprite(const Sprite& s);
  virtual ~Sprite() { } 

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;

  virtual void update(Uint32 ticks);
  virtual void explode();
  Sprite& operator=(const Sprite& rhs);
  
  virtual bool isExplosion() const { if(explosion) return true; return false; };

private:
  ExplodingSprite* explosion;
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const Sprite*) const;
};
#endif
