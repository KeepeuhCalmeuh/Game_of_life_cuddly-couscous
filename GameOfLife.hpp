#pragma once
#include <unordered_set>
#include <utility>

struct CellHash {
    std::size_t operator()(const std::pair<int, int>& c) const {
        return std::hash<int>()(c.first) ^ (std::hash<int>()(c.second) << 1);
    }
};

class GameOfLife {
public:
    GameOfLife();
    void randomize(int minX, int minY, int maxX, int maxY);
    void update();
    bool isAlive(int x, int y) const;
    void setCell(int x, int y, bool alive);
    void clear();
    const std::unordered_set<std::pair<int, int>, CellHash>& getCells() const;
    int getGeneration() const { return generation; }
    void resetGeneration() { generation = 0; }
private:
    std::unordered_set<std::pair<int, int>, CellHash> aliveCells;
    int generation = 0;
};
