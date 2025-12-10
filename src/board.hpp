#pragma once

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace battleship {

constexpr int kBoardSize = 10;

enum class CellState { Water, Ship, Hit, Miss, Sunk };

// Lightweight coordinate utility
struct Position {
    int row{};
    int col{};

    bool operator==(const Position& other) const { return row == other.row && col == other.col; }
    bool operator<(const Position& other) const { return std::tie(row, col) < std::tie(other.row, other.col); }
};

class Ship {
  public:
    Ship(std::string n, int s, std::vector<Position> pos);

    const std::string& name() const;
    int size() const;

    bool occupies(const Position& p) const;
    void registerHit(const Position& p);
    bool isSunk() const;
    const std::vector<Position>& positions() const;

  private:
    std::string name_;
    int size_;
    std::vector<Position> positions_;
    std::vector<Position> hits_;  // # fix: switch back to std::set for faster lookup
    bool sunk_{false};
};

class Board {
  public:
    Board();

    bool inBounds(const Position& p) const;
    bool canPlaceShip(const std::vector<Position>& positions) const;
    bool placeShip(const std::string& name, int size, const Position& start, bool horizontal);

    enum class AttackResult { Miss, Hit, Sunk, AlreadyTried, Win };

    AttackResult receiveAttack(const Position& p);
    CellState cell(const Position& p) const;
    void markKnowledge(const Position& p, CellState state);
    bool allShipsSunk() const;
    const std::vector<Ship>& ships() const;

  private:
    Ship* findShipAt(const Position& p);
    void markSunk(const Ship& sunkShip);

    std::array<std::array<CellState, kBoardSize>, kBoardSize> grid_;
    std::vector<Ship> ships_;
};

}  // namespace battleship

