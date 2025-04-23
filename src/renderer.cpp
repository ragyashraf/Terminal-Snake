#include "renderer.h"
#include <algorithm>
#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

Renderer::Renderer() 
    : width(0), 
      height(0), 
      initialized(false) {
}

Renderer::~Renderer() {
    cleanup();
}

void Renderer::initialize(int w, int h) {
    width = w;
    height = h;
    
    // Create and initialize the buffer
    buffer.resize(height, std::vector<char>(width, ' '));
    clearBuffer();
    
    // Set up console
    std::cout << "\033[2J"; // Clear screen
    std::cout << "\033[?25l"; // Hide cursor
    
    initialized = true;
}

void Renderer::cleanup() {
    if (initialized) {
        // Restore cursor
        std::cout << "\033[?25h";
        initialized = false;
    }
}

void Renderer::clearBuffer() {
    // Clear the buffer with spaces
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            buffer[y][x] = ' ';
        }
    }
}

void Renderer::clear() {
    clearBuffer();
}

void Renderer::refresh() {
    // Move cursor to top-left corner
    std::cout << "\033[H";
    
    // Render the buffer
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << buffer[y][x];
        }
        std::cout << "\n";
    }
    
    // Flush output
    std::cout.flush();
}

void Renderer::drawChar(int x, int y, char ch, ColorPair colorPair) {
    // Ensure coordinates are within bounds
    if (x >= 0 && x < width && y >= 0 && y < height) {
        buffer[y][x] = ch;
    }
    
    // ColorPair is not used in this implementation but kept for interface compatibility
    (void)colorPair;
}

void Renderer::drawText(int x, int y, const std::string& text, ColorPair colorPair) {
    // Ensure coordinates are within bounds
    if (y >= 0 && y < height) {
        for (size_t i = 0; i < text.length(); ++i) {
            // Since i is unsigned, we don't need to check if x + i >= 0
            if (x + static_cast<int>(i) < width && x >= 0) {
                buffer[y][x + i] = text[i];
            }
        }
    }
    
    // ColorPair is not used in this implementation but kept for interface compatibility
    (void)colorPair;
}

void Renderer::drawBorder() {
    // Draw horizontal borders
    for (int x = 0; x < width; ++x) {
        drawChar(x, 0, '-', ColorPair::BORDER);
        drawChar(x, height - 1, '-', ColorPair::BORDER);
    }
    
    // Draw vertical borders
    for (int y = 0; y < height; ++y) {
        drawChar(0, y, '|', ColorPair::BORDER);
        drawChar(width - 1, y, '|', ColorPair::BORDER);
    }
    
    // Draw corners
    drawChar(0, 0, '+', ColorPair::BORDER);
    drawChar(width - 1, 0, '+', ColorPair::BORDER);
    drawChar(0, height - 1, '+', ColorPair::BORDER);
    drawChar(width - 1, height - 1, '+', ColorPair::BORDER);
}

void Renderer::drawRect(int x, int y, int w, int h, ColorPair colorPair) {
    // Draw horizontal borders
    for (int i = x; i < x + w; ++i) {
        drawChar(i, y, '-', colorPair);
        drawChar(i, y + h - 1, '-', colorPair);
    }
    
    // Draw vertical borders
    for (int i = y; i < y + h; ++i) {
        drawChar(x, i, '|', colorPair);
        drawChar(x + w - 1, i, '|', colorPair);
    }
    
    // Draw corners
    drawChar(x, y, '+', colorPair);
    drawChar(x + w - 1, y, '+', colorPair);
    drawChar(x, y + h - 1, '+', colorPair);
    drawChar(x + w - 1, y + h - 1, '+', colorPair);
}

void Renderer::initializeColors() {
    // No colors needed for the console version
}
