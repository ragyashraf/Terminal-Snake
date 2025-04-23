#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include <iostream>

enum class ColorPair {
    DEFAULT,
    BORDER,
    SNAKE_HEAD,
    SNAKE_BODY_1,
    SNAKE_BODY_2,
    FOOD_BRIGHT,
    FOOD_MEDIUM,
    FOOD_DIM,
    FOOD_DARK,
    SCORE,
    TITLE,
    SUBTITLE,
    MENU_NORMAL,
    MENU_HIGHLIGHT,
    EXPLOSION_BRIGHT,
    EXPLOSION_MEDIUM,
    EXPLOSION_DARK,
    DEATH,
    DEATH_DARK,
    COUNT  // Keep this last for counting
};

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    void initialize(int width, int height);
    void cleanup();
    
    void clear();
    void refresh();
    
    void drawChar(int x, int y, char ch, ColorPair colorPair = ColorPair::DEFAULT);
    void drawText(int x, int y, const std::string& text, ColorPair colorPair = ColorPair::DEFAULT);
    void drawBorder();
    void drawRect(int x, int y, int width, int height, ColorPair colorPair = ColorPair::DEFAULT);
    
private:
    int width;
    int height;
    bool initialized;
    std::vector<std::vector<char>> buffer;
    
    void clearBuffer();
    void initializeColors();
};

#endif // RENDERER_H
