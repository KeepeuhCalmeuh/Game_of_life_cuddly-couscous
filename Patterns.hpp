#pragma once
#include <vector>
#include <utility>

// Chaque motif est un vecteur de positions relatives (x, y) à placer autour du point d'insertion
namespace Patterns {
    using Pattern = std::vector<std::pair<int, int>>;

    // Glider
    const Pattern glider = {
        {0, 0}, {1, 0}, {2, 0}, {2, 1}, {1, 2}
    };

    // Blinker
    const Pattern blinker = {
        {0, 0}, {1, 0}, {2, 0}
    };

    // Toad
    const Pattern toad = {
        {1, 0}, {2, 0}, {3, 0}, {0, 1}, {1, 1}, {2, 1}
    };

    // Beacon
    const Pattern beacon = {
        {0, 0}, {1, 0}, {0, 1}, {1, 1}, {2, 2}, {3, 2}, {2, 3}, {3, 3}
    };

    // Pulsar (centre en haut à gauche du motif)
    const Pattern pulsar = {
        {2,0},{3,0},{4,0},{8,0},{9,0},{10,0},
        {0,2},{5,2},{7,2},{12,2},
        {0,3},{5,3},{7,3},{12,3},
        {0,4},{5,4},{7,4},{12,4},
        {2,5},{3,5},{4,5},{8,5},{9,5},{10,5},
        {2,7},{3,7},{4,7},{8,7},{9,7},{10,7},
        {0,8},{5,8},{7,8},{12,8},
        {0,9},{5,9},{7,9},{12,9},
        {0,10},{5,10},{7,10},{12,10},
        {2,12},{3,12},{4,12},{8,12},{9,12},{10,12}
    };

    // Ajoute d'autres motifs ici si besoin
}
