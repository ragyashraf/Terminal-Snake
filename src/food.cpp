#include "food.h"
#include <cmath>

Food::Food() 
    : x(0), 
      y(0), 
      animationTime(0.0f),
      blinkRate(3.0f),  // Blinks per second
      glowAmount(0.0f) {
}

void Food::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
    
    // Reset animation
    animationTime = 0.0f;
    glowAmount = 0.0f;
}

void Food::update(float deltaTime) {
    // Update animation time
    animationTime += deltaTime;
    
    // Calculate glow amount using sine wave for pulsing effect
    glowAmount = (std::sin(animationTime * blinkRate * 2 * M_PI) + 1.0f) / 2.0f;
}

void Food::render(Renderer& renderer) {
    // Choose color based on glow amount
    ColorPair color;
    if (glowAmount > 0.8f) {
        color = ColorPair::FOOD_BRIGHT;
    } else if (glowAmount > 0.5f) {
        color = ColorPair::FOOD_MEDIUM;
    } else if (glowAmount > 0.2f) {
        color = ColorPair::FOOD_DIM;
    } else {
        color = ColorPair::FOOD_DARK;
    }
    
    // Choose character based on glow amount
    char foodChar;
    if (glowAmount > 0.8f) {
        foodChar = '@';
    } else if (glowAmount > 0.5f) {
        foodChar = '&';
    } else if (glowAmount > 0.2f) {
        foodChar = '%';
    } else {
        foodChar = '#';
    }
    
    // Draw the food
    renderer.drawChar(x, y, foodChar, color);
}

int Food::getX() const {
    return x;
}

int Food::getY() const {
    return y;
}
