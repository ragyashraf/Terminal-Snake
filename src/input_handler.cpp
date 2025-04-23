#include "input_handler.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

// Constants for special keys
const int ERR_KEY = -1;
const int KEY_UP_VALUE = 1001;
const int KEY_DOWN_VALUE = 1002;
const int KEY_LEFT_VALUE = 1003;
const int KEY_RIGHT_VALUE = 1004;
const int KEY_ENTER_VALUE = 1005;

// Helper function to read a character from stdin without blocking
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    // Save old terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    
    // Set new terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Set non-blocking mode
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    // Read character
    ch = getchar();
    
    // Restore old terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) {
        return ch;
    }
    
    return ERR_KEY;
}

InputHandler::InputHandler() 
    : lastKey(ERR_KEY) {
}

void InputHandler::initialize() {
    // Set terminal to non-canonical mode
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

Direction InputHandler::getDirection() {
    // Read the key
    int key = kbhit();
    
    if (key != ERR_KEY) {
        lastKey = key;
    }
    
    // Map key to direction
    switch (lastKey) {
        case 'w':
        case 'W':
            return Direction::UP;
            
        case 's':
        case 'S':
            return Direction::DOWN;
            
        case 'a':
        case 'A':
            return Direction::LEFT;
            
        case 'd':
        case 'D':
            return Direction::RIGHT;
            
        default:
            return Direction::NONE;
    }
}

bool InputHandler::isKeyPressed() {
    int key = kbhit();
    if (key != ERR_KEY) {
        lastKey = key;
        return true;
    }
    return false;
}

bool InputHandler::isUpPressed() {
    return checkKey('w') || checkKey('W');
}

bool InputHandler::isDownPressed() {
    return checkKey('s') || checkKey('S');
}

bool InputHandler::isLeftPressed() {
    return checkKey('a') || checkKey('A');
}

bool InputHandler::isRightPressed() {
    return checkKey('d') || checkKey('D');
}

bool InputHandler::isPausePressed() {
    return checkKey('p') || checkKey('P');
}

bool InputHandler::isQuitPressed() {
    return checkKey('q') || checkKey('Q');
}

bool InputHandler::isEnterPressed() {
    return checkKey('\n') || checkKey('\r');
}

void InputHandler::clearKeys() {
    lastKey = ERR_KEY;
    
    // Clear any remaining input
    int key;
    while ((key = kbhit()) != ERR_KEY) {
        // Just drain the input buffer
    }
}

bool InputHandler::checkKey(int key) {
    int input = kbhit();
    if (input != ERR_KEY) {
        lastKey = input;
    }
    
    return lastKey == key;
}
