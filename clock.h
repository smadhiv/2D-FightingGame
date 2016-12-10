#include <SDL.h>
#include <string>
#include <deque>

class Manager;

class Clock {
public:
  static Clock& getInstance();
  unsigned int getTicks() const;
  unsigned int getTotalTicks() const { return sumOfAllTicks; }
  unsigned int getSeconds() const { return getTicks()/1000;  }
  unsigned int getFrameRate() const  { return frameRate;  }
  
private:
  friend class Manager;

  bool started;
  bool paused;
  bool sloMo;
  const bool framesAreCapped;
  const unsigned int frameCap;

  unsigned int frames;

  unsigned int tickSum;
  unsigned int sumOfAllTicks;
  unsigned int timeAtStart;
  unsigned int timeAtPause;
  unsigned int currTicks;
  unsigned int prevTicks;
  unsigned int ticks;
  std::deque<unsigned int> timeEachFrame;
  unsigned int frameRate;
  unsigned int fpsAveragedOver;
  
  unsigned int getElapsedTicks();
  Clock& operator++();
  void toggleSloMo();

  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  unsigned int getFrames() const  { return frames;  }
  //unsigned int getFrameRate() const  { return frameRate;  }
  //unsigned int getSeconds() const { return getTicks()/1000;  }
  unsigned int capFrameRate() const;
  void computeFrameRate();

  void start();
  void pause();
  void unpause();
  void display() const;

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
