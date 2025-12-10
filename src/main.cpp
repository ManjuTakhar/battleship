// Command-line Battleship game in modern C++
// Provides simple interactive loop for a single human vs computer opponent.
// Ships are placed randomly with validation; the player enters coordinates like A5.
// The code includes explicit # fix comments marking places for future improvement work.

#include "game.hpp"
#include <exception>
#include <iostream>

int main() {
    try {
        battleship::Game game;
        // # fix: add CLI args for custom board sizes
        game.setup();
        game.run();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}

