#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "snake.h"

class InputHandler {
public:
    InputHandler();
    
    void initialize();
    Direction getDirection();
    
    bool isKeyPressed();
    bool isUpPressed();
    bool isDownPressed();
    bool isLeftPressed();
    bool isRightPressed();
    bool isPausePressed();
    bool isQuitPressed();
    bool isEnterPressed();
    
    void clearKeys();
    
private:
    int lastKey;
    
    bool checkKey(int key);
};

#endif // INPUT_HANDLER_H
