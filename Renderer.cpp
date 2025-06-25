#include "Renderer.hpp"
#include "Camera.hpp"
#include <algorithm>
#include <cmath>

Renderer::Renderer(int windowWidth, int windowHeight, int cellSize)
    : windowWidth(windowWidth), windowHeight(windowHeight), cellSize(cellSize) {}

void Renderer::draw(const GameOfLife& game, const Camera& camera) {
    float zoom = camera.getZoom();
    float offsetX = camera.getX();
    float offsetY = camera.getY();

    // Calcul de la zone visible en coordonnées grille
    int minCellX = static_cast<int>(std::floor((offsetX - windowWidth / (2 * zoom)) / cellSize)) - 2;
    int maxCellX = static_cast<int>(std::ceil((offsetX + windowWidth / (2 * zoom)) / cellSize)) + 2;
    int minCellY = static_cast<int>(std::floor((offsetY - windowHeight / (2 * zoom)) / cellSize)) - 2;
    int maxCellY = static_cast<int>(std::ceil((offsetY + windowHeight / (2 * zoom)) / cellSize)) + 2;

    // Dessin des cellules vivantes
    glBegin(GL_QUADS);
    for (const auto& cell : game.getCells()) {
        int x = cell.first;
        int y = cell.second;
        if (x < minCellX || x > maxCellX || y < minCellY || y > maxCellY) continue;
        float fx = ((x * cellSize - offsetX) / (windowWidth * 0.5f));
        float fy = ((y * cellSize - offsetY) / (windowHeight * 0.5f));
        float fx2 = (((x+1) * cellSize - offsetX) / (windowWidth * 0.5f));
        float fy2 = (((y+1) * cellSize - offsetY) / (windowHeight * 0.5f));
        fx *= zoom; fy *= zoom; fx2 *= zoom; fy2 *= zoom;
        glColor3f(0.2f, 0.8f, 0.2f);
        glVertex2f(fx, fy);
        glVertex2f(fx2, fy);
        glVertex2f(fx2, fy2);
        glVertex2f(fx, fy2);
    }
    glEnd();

    // Dessin de la grille infinie
    glColor3f(0.0f, 0.0f, 0.0f); // grille noire
    glBegin(GL_LINES);
    for (int x = minCellX; x <= maxCellX; ++x) {
        float fx = ((x * cellSize - offsetX) / (windowWidth * 0.5f)) * zoom;
        glVertex2f(fx, ((minCellY * cellSize - offsetY) / (windowHeight * 0.5f)) * zoom);
        glVertex2f(fx, ((maxCellY * cellSize - offsetY) / (windowHeight * 0.5f)) * zoom);
    }
    for (int y = minCellY; y <= maxCellY; ++y) {
        float fy = ((y * cellSize - offsetY) / (windowHeight * 0.5f)) * zoom;
        glVertex2f(((minCellX * cellSize - offsetX) / (windowWidth * 0.5f)) * zoom, fy);
        glVertex2f(((maxCellX * cellSize - offsetX) / (windowWidth * 0.5f)) * zoom, fy);
    }
    glEnd();
}
