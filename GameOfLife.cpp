#include "GameOfLife.hpp"
#include <cstdlib>
#include <ctime>
#include <unordered_map>

GameOfLife::GameOfLife() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void GameOfLife::randomize(int minX, int minY, int maxX, int maxY) {
    for (int y = minY; y < maxY; ++y)
        for (int x = minX; x < maxX; ++x)
            if (std::rand() % 2)
                aliveCells.insert({x, y});
}

void GameOfLife::update() {
    std::unordered_map<std::pair<int, int>, int, CellHash> neighborCount;
    for (const auto& cell : aliveCells) {
        int x = cell.first, y = cell.second;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                neighborCount[{x + dx, y + dy}]++;
            }
        }
    }
    std::unordered_set<std::pair<int, int>, CellHash> next;
    for (const auto& [cell, count] : neighborCount) {
        if (count == 3 || (count == 2 && aliveCells.count(cell)))
            next.insert(cell);
    }
    aliveCells.swap(next);
    generation++; // Incrémente le compteur de génération
}

bool GameOfLife::isAlive(int x, int y) const {
    return aliveCells.count({x, y}) > 0;
}

void GameOfLife::setCell(int x, int y, bool alive) {
    if (alive)
        aliveCells.insert({x, y});
    else
        aliveCells.erase({x, y});
}

void GameOfLife::clear() {
    aliveCells.clear();
    generation = 0; // Remet à zéro le compteur de génération
}

const std::unordered_set<std::pair<int, int>, CellHash>& GameOfLife::getCells() const {
    return aliveCells;
}
