#include "snake.h"
#include <cmath>

Snake::Snake() 
    : currentDirection(Direction::RIGHT),
      queuedDirection(Direction::NONE),
      growing(false),
      growthAmount(0),
      moveProgress(0.0f) {
}

void Snake::initialize(int startX, int startY) {
    body.clear();
    
    // Create initial snake with 3 segments
    for (int i = 0; i < 3; i++) {
        SnakeSegment segment;
        segment.x = startX - i;
        segment.y = startY;
        segment.direction = Direction::RIGHT;
        body.push_back(segment);
    }
    
    currentDirection = Direction::RIGHT;
    queuedDirection = Direction::NONE;
    growing = false;
    growthAmount = 0;
    moveProgress = 0.0f;
}

void Snake::update() {
    // Increment movement progress for smooth animation
    moveProgress += MOVE_SPEED / 60.0f;  // Assuming ~60 frames per second
    
    // If we've reached the next cell, update the snake position
    if (moveProgress >= 1.0f) {
        moveProgress = 0.0f;
        
        // Apply queued direction change if valid
        if (queuedDirection != Direction::NONE) {
            if (isValidDirectionChange(currentDirection, queuedDirection)) {
                currentDirection = queuedDirection;
            }
            queuedDirection = Direction::NONE;
        }
        
        // Move snake body
        SnakeSegment newHead = body.front();
        
        // Update head position based on direction
        switch (currentDirection) {
            case Direction::UP:
                newHead.y--;
                break;
            case Direction::DOWN:
                newHead.y++;
                break;
            case Direction::LEFT:
                newHead.x--;
                break;
            case Direction::RIGHT:
                newHead.x++;
                break;
            default:
                break;
        }
        
        newHead.direction = currentDirection;
        body.push_front(newHead);
        
        // Remove tail if not growing
        if (growing) {
            growthAmount--;
            if (growthAmount <= 0) {
                growing = false;
            }
        } else {
            body.pop_back();
        }
    }
    
    // Update segment positions for smooth animation
    updateSegmentPositions();
}

void Snake::render(Renderer& renderer) {
    // Draw each snake segment
    for (size_t i = 0; i < body.size(); i++) {
        const auto& segment = body[i];
        
        // Calculate display position with interpolation for smooth movement
        float displayX, displayY;
        
        if (i == 0) {
            // Head - move smoothly in the current direction
            displayX = segment.x;
            displayY = segment.y;
            
            switch (currentDirection) {
                case Direction::UP:
                    displayY += moveProgress;
                    break;
                case Direction::DOWN:
                    displayY -= moveProgress;
                    break;
                case Direction::LEFT:
                    displayX += moveProgress;
                    break;
                case Direction::RIGHT:
                    displayX -= moveProgress;
                    break;
                default:
                    break;
            }
        } else {
            // Body segments follow the previous segment
            displayX = segment.x;
            displayY = segment.y;
        }
        
        // Choose character based on segment position
        char ch;
        if (i == 0) {
            // Head character based on direction
            switch (currentDirection) {
                case Direction::UP:    ch = '^'; break;
                case Direction::DOWN:  ch = 'v'; break;
                case Direction::LEFT:  ch = '<'; break;
                case Direction::RIGHT: ch = '>'; break;
                default:               ch = 'O'; break;
            }
        } else if (i == body.size() - 1) {
            // Tail character
            ch = '*';
        } else {
            // Body character
            ch = 'o';
        }
        
        // Draw the segment with color
        ColorPair color;
        if (i == 0) {
            color = ColorPair::SNAKE_HEAD;
        } else if (i % 2 == 0) {
            color = ColorPair::SNAKE_BODY_1;
        } else {
            color = ColorPair::SNAKE_BODY_2;
        }
        
        renderer.drawChar(static_cast<int>(displayX), static_cast<int>(displayY), ch, color);
    }
}

void Snake::renderDeath(Renderer& renderer, int frame, int maxFrames) {
    // Death animation - explosion effect
    const float progress = static_cast<float>(frame) / maxFrames;
    
    for (size_t i = 0; i < body.size(); i++) {
        const auto& segment = body[i];
        
        // Only process segments that should be visible in this frame
        float segmentDelay = static_cast<float>(i) / body.size() * 0.5f;
        float segmentProgress = progress - segmentDelay;
        
        if (segmentProgress <= 0) {
            // Segment hasn't started exploding yet - render normally
            char ch = (i == 0) ? 'X' : 'x';
            ColorPair color = (i == 0) ? ColorPair::SNAKE_HEAD : 
                             (i % 2 == 0 ? ColorPair::SNAKE_BODY_1 : ColorPair::SNAKE_BODY_2);
            
            renderer.drawChar(static_cast<int>(segment.x), static_cast<int>(segment.y), ch, color);
        } else if (segmentProgress < 1.0f) {
            // Segment is exploding
            const int explosionRadius = static_cast<int>(segmentProgress * 3);
            
            // Draw explosion particles
            for (int dy = -explosionRadius; dy <= explosionRadius; dy++) {
                for (int dx = -explosionRadius; dx <= explosionRadius; dx++) {
                    // Create circular explosion shape
                    float distance = std::sqrt(dx * dx + dy * dy);
                    if (distance <= explosionRadius && distance >= explosionRadius - 1.0f) {
                        int x = static_cast<int>(segment.x) + dx;
                        int y = static_cast<int>(segment.y) + dy;
                        
                        // Choose explosion character
                        char explChar;
                        if (rand() % 3 == 0) explChar = '*';
                        else if (rand() % 2 == 0) explChar = '+';
                        else explChar = '.';
                        
                        ColorPair color;
                        float intensity = 1.0f - segmentProgress;
                        
                        if (intensity > 0.7f) color = ColorPair::EXPLOSION_BRIGHT;
                        else if (intensity > 0.4f) color = ColorPair::EXPLOSION_MEDIUM;
                        else color = ColorPair::EXPLOSION_DARK;
                        
                        renderer.drawChar(x, y, explChar, color);
                    }
                }
            }
        }
        // If segmentProgress >= 1.0, the segment has fully exploded and disappears
    }
}

void Snake::changeDirection(Direction newDirection) {
    // Queue the direction change for the next update
    queuedDirection = newDirection;
}

bool Snake::checkFoodCollision(const Food& food) {
    // Check if head overlaps with food
    int headX = getHeadX();
    int headY = getHeadY();
    
    return (headX == food.getX() && headY == food.getY());
}

bool Snake::checkSelfCollision() {
    // Check if head collides with any other segment
    int headX = getHeadX();
    int headY = getHeadY();
    
    // Start from the 4th segment (index 3) to avoid false collisions
    // with segments that are too close to the head
    for (size_t i = 3; i < body.size(); i++) {
        if (static_cast<int>(body[i].x) == headX && 
            static_cast<int>(body[i].y) == headY) {
            return true;
        }
    }
    
    return false;
}

void Snake::grow() {
    growing = true;
    growthAmount += static_cast<int>(GROWTH_FACTOR);
}

bool Snake::containsPosition(int x, int y) const {
    for (const auto& segment : body) {
        if (static_cast<int>(segment.x) == x && static_cast<int>(segment.y) == y) {
            return true;
        }
    }
    
    return false;
}

int Snake::getHeadX() const {
    return static_cast<int>(body.front().x);
}

int Snake::getHeadY() const {
    return static_cast<int>(body.front().y);
}

Direction Snake::getOppositeDirection(Direction dir) const {
    switch (dir) {
        case Direction::UP:    return Direction::DOWN;
        case Direction::DOWN:  return Direction::UP;
        case Direction::LEFT:  return Direction::RIGHT;
        case Direction::RIGHT: return Direction::LEFT;
        default:               return Direction::NONE;
    }
}

bool Snake::isValidDirectionChange(Direction current, Direction newDir) const {
    // Can't reverse direction directly
    return newDir != getOppositeDirection(current);
}

void Snake::updateSegmentPositions() {
    // This method provides the smooth animation effect
    // by interpolating between integer positions
    for (size_t i = 1; i < body.size(); i++) {
        SnakeSegment& prev = body[i-1];
        SnakeSegment& curr = body[i];
        
        // Interpolate positions for smooth movement
        float dx = prev.x - curr.x;
        float dy = prev.y - curr.y;
        
        // If distance is more than 1.5, there's a gap - probably a wrap-around situation
        if (std::abs(dx) > 1.5f || std::abs(dy) > 1.5f) {
            // Don't interpolate in this case
            continue;
        }
        
        // Update segment direction based on relative position to previous segment
        if (std::abs(dx) > std::abs(dy)) {
            curr.direction = (dx > 0) ? Direction::RIGHT : Direction::LEFT;
        } else if (std::abs(dy) > 0) {
            curr.direction = (dy > 0) ? Direction::DOWN : Direction::UP;
        }
    }
}
