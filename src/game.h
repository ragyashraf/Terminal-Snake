#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "food.h"
#include "renderer.h"
#include "input_handler.h"
#include <string>
#include <chrono>
#include <fstream>
#include <vector>

enum class GameState {
    INTRO,
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    QUIT
};

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD,
    EXTREME
};

struct HighScore {
    std::string name;
    int score;
    Difficulty difficulty;
};

class Game {
public:
    Game();
    ~Game();
    
    void run();
    void cleanup();
    
private:
    // Game components
    Snake snake;
    Food food;
    Renderer renderer;
    InputHandler input;
    
    // Game state
    GameState state;
    Difficulty difficulty;
    int score;
    int highScore;
    std::vector<HighScore> highScores;
    bool gameRunning;
    float gameSpeed;
    int level;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastUpdateTime;
    float frameTime;  // Time in seconds for each frame
    
    // Game dimensions
    int width;
    int height;
    
    // Game logic
    void initialize();
    void processInput();
    void update();
    bool checkCollision();
    void render();
    void generateFood();
    
    // Game state handlers
    void handleIntro();
    void handleMenu();
    void handlePlaying();
    void handlePaused();
    void handleGameOver();
    
    // Utility functions
    void loadHighScores();
    void saveHighScore();
    void resetGame();
    void updateDifficulty(Difficulty newDifficulty);
    float getDifficultyMultiplier() const;
    void incrementLevel();
    void addObstacles();
    std::string getDifficultyString() const;
};

#endif // GAME_H
