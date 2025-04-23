#include "game.h"
#include <iostream>
#include <csignal>

Game* gameInstance = nullptr;

void signalHandler(int signum) {
    if (gameInstance) {
        gameInstance->cleanup();
    }
    exit(signum);
}

int main() {
    // Set up signal handling for clean exit
    signal(SIGINT, signalHandler);
    
    try {
        Game game;
        gameInstance = &game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
