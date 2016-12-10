#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "HUD.h"
#include "scaledSprite.h"
#include "player.h"
#include "subjectplayer.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();
  void printVel();
  
private:
  const bool env;
  bool displayHUD;
    const Gamedata& gdata;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  World layer1;
  World layer2;
  SDL_Surface * const orbSurface;
  SDL_Surface * const backSurface;
  Frame backFrame;
  Viewport& viewport;

  std::vector<Drawable* > sprites;
  std::vector<ScaledSprite* > orbs;
  Player *players;
  HUD hud;
  int currentSprite;

  bool makeVideo;
  unsigned int frameCount;
  const std::string username;
  const std::string title;
  const unsigned int frameMax;

  void makeOrbs();
  void draw() const;
  void update();
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};


