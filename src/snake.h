#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <deque>
#include "food.h"
#include "renderer.h"

enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct SnakeSegment {
    float x;
    float y;
    Direction direction;
};

class Snake {
public:
    Snake();
    
    void initialize(int startX, int startY);
    void update();
    void render(Renderer& renderer);
    void renderDeath(Renderer& renderer, int frame, int maxFrames);
    void changeDirection(Direction newDirection);
    bool checkFoodCollision(const Food& food);
    bool checkSelfCollision();
    void grow();
    
    bool containsPosition(int x, int y) const;
    
    // Getters
    int getHeadX() const;
    int getHeadY() const;
    
private:
    std::deque<SnakeSegment> body;
    Direction currentDirection;
    Direction queuedDirection;
    bool growing;
    int growthAmount;
    float moveProgress;  // 0.0 to 1.0, for smooth animation
    
    // Animation settings
    static constexpr float MOVE_SPEED = 8.0f;  // segments per second
    static constexpr float GROWTH_FACTOR = 3.0f;
    
    // Helper methods
    Direction getOppositeDirection(Direction dir) const;
    bool isValidDirectionChange(Direction current, Direction newDir) const;
    void updateSegmentPositions();
};

#endif // SNAKE_H
