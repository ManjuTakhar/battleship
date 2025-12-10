#pragma once

#include "board.hpp"

#include <chrono>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace battleship {

class Random {
  public:
    Random() : rng_(std::random_device{}()) {}
    int nextInt(int min, int max) { return std::uniform_int_distribution<int>(min, max)(rng_); }
    bool nextBool() { return std::bernoulli_distribution(0.5)(rng_); }
    // # fix: expose deterministic seed for reproducible games

  private:
    std::mt19937 rng_;
};

class Player {
  public:
    explicit Player(std::string name) : name_(std::move(name)) {}

    void autoPlaceShips(Random& random);
    Board::AttackResult receiveAttack(const Position& p);
    bool defeated() const;
    const Board& board() const;
    Board& board();
    const std::string& name() const;

  private:
    struct ShipDef {
        std::string name;
        int size;
    };
    static const std::vector<ShipDef>& shipDefinitions();

    std::string name_;
    Board board_;
};

class CliView {
  public:
    static void printBoards(const Board& tracking, const Board& own);
    static void printBoard(const Board& board, bool hideShips);
    static std::optional<Position> parseInput(const std::string& input);

  private:
    static char cellSymbol(const Board& board, const Position& p, bool hideShips);
};

class Game {
  public:
    Game();
    void setup();
    void run();

  private:
    Position promptPlayer();
    static std::string coordString(const Position& p);
    Position randomEnemyShot(Random& random);
    void updateTracking(Board& tracking, const Position& p, Board::AttackResult result);
    void reportResult(const std::string& defender, Board::AttackResult result);

    Player human_;
    Player computer_;
};

}  // namespace battleship

