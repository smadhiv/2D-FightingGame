#ifndef SUBJECTPLAYER__H
#define SUBJECTPLAYER__H
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "smartplayer.h"
#include "player.h"

class SubjectPlayer : public Player {
public:
  SubjectPlayer(const std::string& name1, const std::string& name2);
  ~SubjectPlayer();
  
  void update(Uint32 ticks);
  void draw() const;
  void handleKeyStroke(const Uint8 *);

private:
  Player* player2;
  SubjectPlayer& operator=(const SubjectPlayer& );
  SubjectPlayer(const SubjectPlayer& );
  
};
#endif