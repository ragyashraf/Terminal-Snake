#ifndef FOOD_H
#define FOOD_H

#include "renderer.h"

class Food {
public:
    Food();
    
    void setPosition(int x, int y);
    void update(float deltaTime);
    void render(Renderer& renderer);
    
    // Getters
    int getX() const;
    int getY() const;
    
private:
    int x;
    int y;
    float animationTime;
    float blinkRate;   // How fast the food blinks
    float glowAmount;  // Current glow intensity (0.0 to 1.0)
};

#endif // FOOD_H
