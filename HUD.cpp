#include "HUD.h"
#include "ioManager.h"
#include "clock.h"
#include <string>
#include <iostream>

HUD::HUD(float x0, float x1, float x2, float x3, float y0, float y1, float y2, float y3, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float thick): x0(x0), x1(x1), x2(x2), x3(x3), y0(y0), y1(y1), y2(y2), y3(y3), r(r), g(g), b(b), a(a), thick(thick) {}

void HUD::draw(const Player* player1) const {
  Draw_AALine(IOManager::getInstance().getScreen(), x0, y0, x1, y1, 1, 0, 0, 0, a);
  Draw_AALine(IOManager::getInstance().getScreen(), x1, y1, x2, y2, 1, 0, 0, 0, a);
  Draw_AALine(IOManager::getInstance().getScreen(), x2, y2, x3, y3, 1, 0, 0, 0, a);
  Draw_AALine(IOManager::getInstance().getScreen(), x3, y3, x0, y0, 1, 0, 0, 0, a);
  Draw_AALine(IOManager::getInstance().getScreen(), x0 + 1, (y0 + y2) / 2, x1 - 1, (y0 + y2) / 2, y2 - y0, 255, 255, 255, a);
  IOManager::getInstance().printMessageValueAt("Seconds : ", Clock::getInstance().getSeconds(), x0 + 5, 20);
  IOManager::getInstance().printMessageValueAt("FPS       : ", Clock::getInstance().getFrameRate(), x0 + 5, 45);  
  IOManager::getInstance().printMessageAt("Controls            Player1   :  Player2", x0 + 5, 70);
  IOManager::getInstance().printMessageAt("Movement            'a d'     :  'left right'", x0 + 5, 95);
  IOManager::getInstance().printMessageAt("Crouch/Block       's/w'     :  'down/up'", x0 + 5, 110);
  IOManager::getInstance().printMessageAt("Punch/Throw        '1/2'     :  '9/0'", x0 + 5, 125);
  IOManager::getInstance().printMessageAt("God Mode/AI         'g'        :  'h/j'", x0 + 5, 140);
  IOManager::getInstance().printMessageValueAt("Active/Free Objects        : ", std::to_string(player1->getActiveProjectiles())+"/"+std::to_string(player1->getAvailableProjectiles()), x0 + 5, 155);
}



  
