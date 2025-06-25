#pragma once
#include "GameOfLife.hpp"
#include "Camera.hpp"
#include <GLFW/glfw3.h>

class Renderer {
public:
    Renderer(int windowWidth, int windowHeight, int cellSize);
    void draw(const GameOfLife& game, const Camera& camera);
private:
    int windowWidth, windowHeight, cellSize;
};
