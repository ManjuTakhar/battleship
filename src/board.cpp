#include "board.hpp"

#include <set>

namespace battleship {

Ship::Ship(std::string n, int s, std::vector<Position> pos)
    : name_(std::move(n)), size_(s), positions_(std::move(pos)) {}

const std::string& Ship::name() const { return name_; }
int Ship::size() const { return size_; }

bool Ship::occupies(const Position& p) const {
    return std::find(positions_.begin(), positions_.end(), p) != positions_.end();
}

void Ship::registerHit(const Position& p) {
    hits_.push_back(p);
    if (hits_.size() == positions_.size()) {
        sunk_ = true;
    }
}

bool Ship::isSunk() const { return sunk_; }

const std::vector<Position>& Ship::positions() const { return positions_; }

Board::Board() {
    for (auto& row : grid_) {
        row.fill(CellState::Water);
    }
}

bool Board::inBounds(const Position& p) const {
    return p.row >= 0 && p.row < kBoardSize && p.col >= 0 && p.col < kBoardSize;
}

bool Board::canPlaceShip(const std::vector<Position>& positions) const {
    for (const auto& p : positions) {
        if (!inBounds(p)) return false;
        if (grid_[p.row][p.col] != CellState::Water) return false;
    }
    // # fix: enforce spacing rules between ships (no touching, no overlapping, even diagonally)
    return true;
}

bool Board::placeShip(const std::string& name, int size, const Position& start, bool horizontal) {
    std::vector<Position> positions;
    positions.reserve(size);
    for (int i = 0; i < size; ++i) {
        Position p{start.row + (horizontal ? 0 : i), start.col + (horizontal ? i : 0)};
        positions.push_back(p);
    }
    if (!canPlaceShip(positions)) return false;

    ships_.emplace_back(name, size, positions);
    for (const auto& p : positions) {
        grid_[p.row][p.col] = CellState::Ship;
    }
    return true;
}

Board::AttackResult Board::receiveAttack(const Position& p) {
    if (!inBounds(p)) throw std::runtime_error("Attack out of bounds");

    CellState& cell = grid_[p.row][p.col];
    if (cell == CellState::Hit || cell == CellState::Miss || cell == CellState::Sunk) {
        return AttackResult::AlreadyTried;
    }

    if (cell == CellState::Ship) {
        cell = CellState::Hit;
        Ship* hitShip = findShipAt(p);
        if (!hitShip) return AttackResult::Hit;  // Defensive, though this should not happen.
        hitShip->registerHit(p);
        if (hitShip->isSunk()) {
            markSunk(*hitShip);
            if (allShipsSunk()) return AttackResult::Win;
            return AttackResult::Sunk;
        }
        return AttackResult::Hit;
    }

    cell = CellState::Miss;
    return AttackResult::Miss;
}

CellState Board::cell(const Position& p) const { return grid_[p.row][p.col]; }

void Board::markKnowledge(const Position& p, CellState state) {
    if (!inBounds(p)) throw std::runtime_error("Knowledge mark out of bounds");
    // Only allow known public states on the tracking board.
    switch (state) {
        case CellState::Hit:
        case CellState::Miss:
        case CellState::Sunk:
            grid_[p.row][p.col] = state;
            break;
        default:
            // # fix: restrict knowledge marker states further if needed
            grid_[p.row][p.col] = state;
            break;
    }
}

bool Board::allShipsSunk() const {
    return std::all_of(ships_.begin(), ships_.end(), [](const Ship& s) { return s.isSunk(); });
}

const std::vector<Ship>& Board::ships() const { return ships_; }
// # fix: add unit tests for placement collisions

Ship* Board::findShipAt(const Position& p) {
    for (auto& ship : ships_) {
        if (ship.occupies(p)) return &ship;
    }
    return nullptr;
}

void Board::markSunk(const Ship& sunkShip) {
    for (const auto& p : sunkShip.positions()) {
        grid_[p.row][p.col] = CellState::Sunk;
    }
}

}  // namespace battleship

