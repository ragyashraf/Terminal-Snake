#include "game.h"
#include "utils.h"
#include <thread>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cmath>

const std::string HIGH_SCORE_FILE = "snake_high_scores.dat";
const int MAX_HIGH_SCORES = 10;
const int INTRO_DURATION_MS = 2000;
const int FRAME_DELAY_MS = 10;  // 10ms per render frame for smooth animation

Game::Game() 
    : state(GameState::INTRO),
      difficulty(Difficulty::MEDIUM),
      score(0),
      highScore(0),
      gameRunning(true),
      gameSpeed(1.0f),
      level(1),
      frameTime(0.1f),  // Initial frame time (will be adjusted by difficulty)
      width(80),
      height(24) {
    
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Initialize the game components
    initialize();
    
    // Load high scores
    loadHighScores();
}

Game::~Game() {
    cleanup();
}

void Game::cleanup() {
    renderer.cleanup();
}

void Game::run() {
    // Main game loop
    while (gameRunning) {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Handle game states
        switch (state) {
            case GameState::INTRO:
                handleIntro();
                break;
                
            case GameState::MENU:
                handleMenu();
                break;
                
            case GameState::PLAYING:
                handlePlaying();
                break;
                
            case GameState::PAUSED:
                handlePaused();
                break;
                
            case GameState::GAME_OVER:
                handleGameOver();
                break;
                
            case GameState::QUIT:
                gameRunning = false;
                break;
        }
        
        // Calculate frame time and sleep if needed
        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        
        // Ensure we don't render too fast by maintaining a minimum frame time
        if (elapsed < FRAME_DELAY_MS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_DELAY_MS - elapsed));
        }
    }
}

void Game::initialize() {
    // Initialize renderer
    renderer.initialize(width, height);
    
    // Set up input handler
    input.initialize();
    
    // Set initial snake position and direction
    snake.initialize(width / 2, height / 2);
    
    // Generate initial food
    generateFood();
    
    // Set up initial game speed based on difficulty
    updateDifficulty(difficulty);
    
    // Initialize timing
    lastUpdateTime = std::chrono::high_resolution_clock::now();
}

void Game::processInput() {
    Direction dir = input.getDirection();
    
    if (dir != Direction::NONE) {
        snake.changeDirection(dir);
    }
    
    // Check for pause
    if (input.isPausePressed()) {
        state = GameState::PAUSED;
    }
    
    // Check for quit
    if (input.isQuitPressed()) {
        state = GameState::QUIT;
    }
}

void Game::update() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastUpdateTime).count();
    
    // Update game components at the game speed
    if (deltaTime >= frameTime) {
        // Update snake position
        snake.update();
        
        // Check if snake eats food
        if (snake.checkFoodCollision(food)) {
            snake.grow();
            score += 10 * static_cast<int>(difficulty) + 1;
            
            // Every 5 food items, increase level
            if (score % (50 * (static_cast<int>(difficulty) + 1)) == 0) {
                incrementLevel();
            }
            
            // Generate new food
            generateFood();
            
            // Update high score if needed
            if (score > highScore) {
                highScore = score;
            }
        }
        
        // Check for collisions
        if (checkCollision()) {
            state = GameState::GAME_OVER;
            saveHighScore();
        }
        
        // Animate food
        food.update(deltaTime);
        
        lastUpdateTime = currentTime;
    }
}

bool Game::checkCollision() {
    // Check if snake collides with itself
    if (snake.checkSelfCollision()) {
        return true;
    }
    
    // Check if snake hits the boundaries
    int headX = snake.getHeadX();
    int headY = snake.getHeadY();
    
    if (headX < 1 || headX >= width - 1 || headY < 1 || headY >= height - 1) {
        return true;
    }
    
    return false;
}

void Game::render() {
    // Clear the screen
    renderer.clear();
    
    // Draw borders
    renderer.drawBorder();
    
    // Draw snake
    snake.render(renderer);
    
    // Draw food
    food.render(renderer);
    
    // Draw score
    std::stringstream ss;
    ss << "Score: " << score << " | High Score: " << highScore << " | Level: " << level << " | " << getDifficultyString();
    renderer.drawText(1, 0, ss.str(), ColorPair::SCORE);
    
    // Refresh the screen
    renderer.refresh();
}

void Game::generateFood() {
    // Generate food in a random location that's not occupied by the snake
    int x, y;
    bool validPosition = false;
    
    while (!validPosition) {
        x = utils::randomInt(1, width - 2);
        y = utils::randomInt(1, height - 2);
        
        // Check if the location is not occupied by the snake
        validPosition = !snake.containsPosition(x, y);
    }
    
    food.setPosition(x, y);
}

void Game::handleIntro() {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    
    // Draw intro animation
    renderer.clear();
    
    const std::string title = "TERMINAL SNAKE";
    const std::string subtitle = "The Most Advanced Terminal Snake Game";
    
    // Animated intro - fade in effect
    if (elapsed < INTRO_DURATION_MS) {
        float progress = static_cast<float>(elapsed) / INTRO_DURATION_MS;
        int visibleChars = static_cast<int>(title.length() * progress);
        
        // Display partial title with animation
        renderer.drawText(width / 2 - title.length() / 2, height / 2 - 1, 
                          title.substr(0, visibleChars), ColorPair::TITLE);
        
        if (progress > 0.5f) {
            float subProgress = (progress - 0.5f) * 2.0f;
            int visibleSubChars = static_cast<int>(subtitle.length() * subProgress);
            
            // Display partial subtitle
            renderer.drawText(width / 2 - subtitle.length() / 2, height / 2 + 1, 
                             subtitle.substr(0, visibleSubChars), ColorPair::SUBTITLE);
        }
    } else {
        // Display full intro
        renderer.drawText(width / 2 - title.length() / 2, height / 2 - 1, title, ColorPair::TITLE);
        renderer.drawText(width / 2 - subtitle.length() / 2, height / 2 + 1, subtitle, ColorPair::SUBTITLE);
        
        // Show "Press any key to continue" with pulsing effect
        std::string pressKey = "Press any key to continue";
        float pulse = (std::sin(elapsed * 0.01f) + 1.0f) / 2.0f;
        ColorPair color = pulse > 0.5f ? ColorPair::MENU_HIGHLIGHT : ColorPair::MENU_NORMAL;
        
        renderer.drawText(width / 2 - pressKey.length() / 2, height / 2 + 3, pressKey, color);
        
        // Check for key press
        if (input.isKeyPressed()) {
            state = GameState::MENU;
            input.clearKeys(); // Clear the key buffer
        }
    }
    
    renderer.refresh();
}

void Game::handleMenu() {
    static int selectedOption = 0;
    const int numOptions = 5;
    const std::string options[numOptions] = {
        "Start Game",
        "Difficulty: " + getDifficultyString(),
        "High Scores",
        "How to Play",
        "Quit"
    };
    
    // Process input
    if (input.isUpPressed()) {
        selectedOption = (selectedOption - 1 + numOptions) % numOptions;
        input.clearKeys();
    } else if (input.isDownPressed()) {
        selectedOption = (selectedOption + 1) % numOptions;
        input.clearKeys();
    } else if (input.isLeftPressed() || input.isRightPressed()) {
        // Change difficulty
        if (selectedOption == 1) {
            int diffVal = static_cast<int>(difficulty);
            
            if (input.isLeftPressed()) {
                diffVal = (diffVal - 1 + 4) % 4;
            } else {
                diffVal = (diffVal + 1) % 4;
            }
            
            updateDifficulty(static_cast<Difficulty>(diffVal));
        }
        input.clearKeys();
    } else if (input.isEnterPressed()) {
        // Handle option selection
        switch (selectedOption) {
            case 0: // Start Game
                resetGame();
                state = GameState::PLAYING;
                break;
                
            case 1: // Difficulty already handled in left/right input
                break;
                
            case 2: // High Scores
                // Not implemented in this version
                break;
                
            case 3: // How to Play
                // Not implemented in this version
                break;
                
            case 4: // Quit
                state = GameState::QUIT;
                break;
        }
        input.clearKeys();
    }
    
    // Render menu
    renderer.clear();
    
    // Draw title
    std::string title = "TERMINAL SNAKE";
    renderer.drawText(width / 2 - title.length() / 2, 5, title, ColorPair::TITLE);
    
    // Draw options
    for (int i = 0; i < numOptions; i++) {
        int y = 10 + i * 2;
        ColorPair color = (i == selectedOption) ? ColorPair::MENU_HIGHLIGHT : ColorPair::MENU_NORMAL;
        
        // Add a cursor for selected option
        std::string option = (i == selectedOption) ? "> " + options[i] + " <" : options[i];
        
        renderer.drawText(width / 2 - option.length() / 2, y, option, color);
    }
    
    // Draw footer
    std::string controls = "Controls: Arrow Keys/WASD - Move, P - Pause, Q - Quit";
    renderer.drawText(width / 2 - controls.length() / 2, height - 3, controls, ColorPair::SUBTITLE);
    
    renderer.refresh();
}

void Game::handlePlaying() {
    // Process input
    processInput();
    
    // Update game state
    update();
    
    // Render the game
    render();
}

void Game::handlePaused() {
    // Render the paused game state
    render();
    
    // Draw pause message
    std::string pauseMsg = "GAME PAUSED";
    std::string continueMsg = "Press P to continue, Q to quit";
    
    int pauseMsgX = width / 2 - pauseMsg.length() / 2;
    int pauseMsgY = height / 2 - 1;
    int continueMsgX = width / 2 - continueMsg.length() / 2;
    int continueMsgY = height / 2 + 1;
    
    renderer.drawRect(pauseMsgX - 2, pauseMsgY - 2, pauseMsg.length() + 4, 5, ColorPair::BORDER);
    renderer.drawText(pauseMsgX, pauseMsgY, pauseMsg, ColorPair::MENU_HIGHLIGHT);
    renderer.drawText(continueMsgX, continueMsgY, continueMsg, ColorPair::MENU_NORMAL);
    
    renderer.refresh();
    
    // Handle pause input
    if (input.isPausePressed()) {
        state = GameState::PLAYING;
        input.clearKeys();
    } else if (input.isQuitPressed()) {
        state = GameState::MENU;
        input.clearKeys();
    }
}

void Game::handleGameOver() {
    static int animFrame = 0;
    static int selectedOption = 0;
    static bool animationDone = false;
    const int numOptions = 2;
    const std::string options[numOptions] = {"Play Again", "Return to Menu"};
    
    // Run death animation first
    if (!animationDone) {
        // Death animation frames
        const int maxFrames = 10;
        
        if (animFrame < maxFrames) {
            // Render the game
            renderer.clear();
            renderer.drawBorder();
            
            // Draw exploding snake
            snake.renderDeath(renderer, animFrame, maxFrames);
            
            // Draw food
            food.render(renderer);
            
            // Draw score
            std::stringstream ss;
            ss << "Score: " << score << " | High Score: " << highScore << " | Level: " << level << " | " << getDifficultyString();
            renderer.drawText(1, 0, ss.str(), ColorPair::SCORE);
            
            renderer.refresh();
            
            // Increment animation frame
            animFrame++;
            
            // Slow down the animation
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } else {
            animationDone = true;
        }
    } else {
        // Process game over menu input
        if (input.isUpPressed() || input.isDownPressed()) {
            selectedOption = 1 - selectedOption; // Toggle between 0 and 1
            input.clearKeys();
        } else if (input.isEnterPressed()) {
            if (selectedOption == 0) {
                // Play again
                resetGame();
                state = GameState::PLAYING;
            } else {
                // Return to menu
                state = GameState::MENU;
            }
            
            // Reset animation state for next time
            animFrame = 0;
            animationDone = false;
            
            input.clearKeys();
        }
        
        // Render game over screen
        renderer.clear();
        
        // Draw game over message
        std::string gameOverMsg = "GAME OVER";
        std::string scoreMsg = "Final Score: " + std::to_string(score);
        
        // Add pulsing effect to game over message
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdateTime).count();
        float pulse = (std::sin(elapsed * 0.005f) + 1.0f) / 2.0f;
        
        ColorPair gameOverColor = pulse > 0.5f ? ColorPair::DEATH : ColorPair::DEATH_DARK;
        
        renderer.drawText(width / 2 - gameOverMsg.length() / 2, height / 2 - 4, gameOverMsg, gameOverColor);
        renderer.drawText(width / 2 - scoreMsg.length() / 2, height / 2 - 2, scoreMsg, ColorPair::SCORE);
        
        // Draw options
        for (int i = 0; i < numOptions; i++) {
            int y = height / 2 + 2 + i * 2;
            ColorPair color = (i == selectedOption) ? ColorPair::MENU_HIGHLIGHT : ColorPair::MENU_NORMAL;
            
            // Add a cursor for selected option
            std::string option = (i == selectedOption) ? "> " + options[i] + " <" : options[i];
            
            renderer.drawText(width / 2 - option.length() / 2, y, option, color);
        }
        
        renderer.refresh();
    }
}

void Game::loadHighScores() {
    std::ifstream file(HIGH_SCORE_FILE, std::ios::binary);
    
    if (file.is_open()) {
        int numScores;
        file.read(reinterpret_cast<char*>(&numScores), sizeof(numScores));
        
        highScores.clear();
        for (int i = 0; i < numScores; i++) {
            HighScore score;
            
            // Read name length
            int nameLength;
            file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
            
            // Read name string
            char nameBuffer[256];
            file.read(nameBuffer, nameLength);
            score.name = std::string(nameBuffer, nameLength);
            
            // Read score
            file.read(reinterpret_cast<char*>(&score.score), sizeof(score.score));
            
            // Read difficulty
            int difficultyValue;
            file.read(reinterpret_cast<char*>(&difficultyValue), sizeof(difficultyValue));
            score.difficulty = static_cast<Difficulty>(difficultyValue);
            
            highScores.push_back(score);
        }
        
        file.close();
        
        // Set current high score from the highest in the list
        if (!highScores.empty()) {
            highScore = highScores[0].score;
        }
    }
}

void Game::saveHighScore() {
    // Only save if score is significant
    if (score <= 0) {
        return;
    }
    
    // Create a new high score entry
    HighScore newScore;
    newScore.name = "Player"; // Would typically ask for name, but keeping it simple
    newScore.score = score;
    newScore.difficulty = difficulty;
    
    // Add to the list
    highScores.push_back(newScore);
    
    // Sort high scores in descending order
    std::sort(highScores.begin(), highScores.end(), 
              [](const HighScore& a, const HighScore& b) { return a.score > b.score; });
    
    // Keep only the top MAX_HIGH_SCORES
    if (highScores.size() > MAX_HIGH_SCORES) {
        highScores.resize(MAX_HIGH_SCORES);
    }
    
    // Save to file
    std::ofstream file(HIGH_SCORE_FILE, std::ios::binary);
    
    if (file.is_open()) {
        // Write number of scores
        int numScores = static_cast<int>(highScores.size());
        file.write(reinterpret_cast<const char*>(&numScores), sizeof(numScores));
        
        // Write each score
        for (const auto& score : highScores) {
            // Write name length
            int nameLength = static_cast<int>(score.name.length());
            file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
            
            // Write name string
            file.write(score.name.c_str(), nameLength);
            
            // Write score
            file.write(reinterpret_cast<const char*>(&score.score), sizeof(score.score));
            
            // Write difficulty
            int difficultyValue = static_cast<int>(score.difficulty);
            file.write(reinterpret_cast<const char*>(&difficultyValue), sizeof(difficultyValue));
        }
        
        file.close();
    }
}

void Game::resetGame() {
    // Reset game state
    score = 0;
    level = 1;
    
    // Reset snake
    snake.initialize(width / 2, height / 2);
    
    // Generate new food
    generateFood();
    
    // Reset timing
    lastUpdateTime = std::chrono::high_resolution_clock::now();
    
    // Update difficulty settings
    updateDifficulty(difficulty);
}

void Game::updateDifficulty(Difficulty newDifficulty) {
    difficulty = newDifficulty;
    
    // Adjust frame time based on difficulty
    frameTime = 0.2f / (1.0f + getDifficultyMultiplier() * 0.5f);
}

float Game::getDifficultyMultiplier() const {
    switch (difficulty) {
        case Difficulty::EASY: return 0.8f;
        case Difficulty::MEDIUM: return 1.0f;
        case Difficulty::HARD: return 1.2f;
        case Difficulty::EXTREME: return 1.5f;
        default: return 1.0f;
    }
}

void Game::incrementLevel() {
    level++;
    
    // Make the game faster as levels increase
    frameTime *= 0.95f;
    
    // Could add obstacles here in more complex implementations
    // addObstacles();
}

std::string Game::getDifficultyString() const {
    switch (difficulty) {
        case Difficulty::EASY: return "Easy";
        case Difficulty::MEDIUM: return "Medium";
        case Difficulty::HARD: return "Hard";
        case Difficulty::EXTREME: return "Extreme";
        default: return "Unknown";
    }
}
