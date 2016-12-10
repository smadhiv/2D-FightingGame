#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
using std::string;

GameSound::~GameSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

GameSound::GameSound() : 
  volume(SDL_MIX_MAXVOLUME), 
  currentSound(-1), 
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  sounds(),
  channels() 
{
  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
  sounds.push_back( Mix_LoadWAV("sound/punch.wav") );
  sounds.push_back( Mix_LoadWAV("sound/throw.wav") );
  sounds.push_back( Mix_LoadWAV("sound/splash.wav") );
  sounds.push_back( Mix_LoadWAV("sound/dying.wav") );
  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
}

void GameSound::operator[](int index) {
  if (currentSound >= 0) Mix_HaltChannel(currentSound);
  currentSound = index;
  Mix_VolumeChunk(sounds[index], volume);
  channels[index] = 
     Mix_PlayChannel(channels[index], sounds[index], 0);
}

GameMusic::~GameMusic() {
  std::cout << "Cleaning up music ..." << std::endl;
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}

GameMusic::GameMusic() : 
  volume(SDL_MIX_MAXVOLUME/5), 
  music(NULL),
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  channels() 
{
  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
  music = Mix_LoadMUS("sound/themeMusic.mp3");
  if (!music) throw string("Couldn't load themeMusic.mp3")+Mix_GetError();
  startMusic();
}

void GameMusic::toggleMusic() {
  if( Mix_PausedMusic() ) { 
    Mix_ResumeMusic(); 
  } 
  else { 
    Mix_PauseMusic(); 
  } 
}

void GameMusic::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music, -1);
}

void GameMusic::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}

