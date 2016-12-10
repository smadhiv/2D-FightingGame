#include <iostream>
#include <string>
#include<algorithm>
#include <iomanip>
#include "twowaysprite.h"
#include "player.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "HUD.h"


class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }
};


Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  sprites.clear();
  for (unsigned i = 0; i < orbs.size(); ++i) {
    delete orbs[i];
  }
  orbs.clear();
  delete players;
  SDL_FreeSurface(orbSurface);
  SDL_FreeSurface(backSurface);
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  displayHUD(false),
  gdata( Gamedata::getInstance() ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  layer1("layer1", Gamedata::getInstance().getXmlInt("layer1/factor") ),
  layer2("layer2", Gamedata::getInstance().getXmlInt("layer2/factor") ),
  orbSurface( io.loadAndSet(gdata.getXmlStr("panch/file"), gdata.getXmlBool("panch/transparency")) ),
  backSurface( io.loadAndSet(gdata.getXmlStr("layer2/file"), 
              gdata.getXmlBool("layer1/transparency")) ),
  backFrame(backSurface,
                gdata.getXmlInt("layer1/width"), 
                gdata.getXmlInt("layer1/height"), 
                gdata.getXmlInt("layer1/src/x"), 
                gdata.getXmlInt("layer1/src/y")
  ),
  viewport( Viewport::getInstance() ),
  sprites(),
  orbs(),
  players(new SubjectPlayer("fighter1", "fighter3")),
  hud((float)400, (float)viewport.getWidth()/ 3 + 400, (float)viewport.getWidth()/ 3 + 400, (float)400, (float)5, (float)5, (float)viewport.getHeight()/ 4, (float)viewport.getHeight()/ 4, (uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)100, (float)1),
  currentSprite(0),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  makeOrbs();
  sprites.push_back( new TwoWaySprite("cat") );
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::makeOrbs() {
  unsigned numberOfOrbs = gdata.getXmlInt("numberOfBirds");
  orbs.reserve( numberOfOrbs );
  for (unsigned i = 0; i < numberOfOrbs; ++i) {
    orbs.push_back( new ScaledSprite("panch", orbSurface) );
  }
  sort(orbs.begin(), orbs.end(), ScaledSpriteCompare());
for (unsigned i = 0; i < numberOfOrbs; ++i) {
    orbs[i]->setVelocityFirst();
  }
}

void Manager::draw() const {
  layer1.draw(); 

  for (unsigned i = 0; i < (orbs.size()/2); ++i) {
    orbs[i]->draw();
  }
  layer2.draw();

  for (unsigned i = (orbs.size()/2); i < orbs.size(); ++i) {
    orbs[i]->draw();
  }

  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  
  players->draw();
  io.printMessageAt(title, 10, 550);
  if(displayHUD || clock.getSeconds() < 4) {
    hud.draw(players);
  }
  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  currentSprite = (currentSprite+1) % sprites.size();
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::update() {
  ++(clock);
  
  Uint32 ticks = clock.getElapsedTicks();
  
  clock.computeFrameRate();
  
  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
  }

  for (unsigned i = 0; i < orbs.size(); ++i) {
    orbs[i]->update(ticks);
  }
  
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }
  
  players->update(ticks);

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  layer1.update();
  layer2.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  GameMusic music;
  SDL_Event event;
  bool done = false;
  Uint8 *keystate = SDL_GetKeyState(NULL);
  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      if(event.type == SDL_QUIT){
        done = true;
      }
      else if(event.type == SDL_KEYDOWN){
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
        }
        if ( keystate[SDLK_F1] ) {
          displayHUD = !displayHUD;
        }
        if ( keystate[SDLK_t] ) {
          switchSprite();
        }
        if ( keystate[SDLK_y] ) {
          clock.toggleSloMo();
        }
        if ( keystate[SDLK_p] ) {      
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDLK_SPACE] ) {      
          music.toggleMusic();
        } 
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        players->handleKeyStroke(keystate);
      }
    }
    draw();
    update();
  }
}

void Manager::printVel(){  
  for (unsigned i = 0; i < orbs.size(); ++i) {
    std::cout << orbs[i]->getScale() << std::endl;
  }
} 
