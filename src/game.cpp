#include "game.hpp"

namespace battleship {

void Player::autoPlaceShips(Random& random) {
    for (const auto& def : shipDefinitions()) {
        bool placed = false;
        int guard = 0;
        while (!placed && guard++ < 500) {
            bool horizontal = random.nextBool();
            int row = random.nextInt(0, kBoardSize - (horizontal ? 1 : def.size));
            int col = random.nextInt(0, kBoardSize - (horizontal ? def.size : 1));
            placed = board_.placeShip(def.name, def.size, {row, col}, horizontal);
        }
        if (!placed) throw std::runtime_error("Failed to place ships");
    }
    // # fix: allow manual ship placement
}

Board::AttackResult Player::receiveAttack(const Position& p) { return board_.receiveAttack(p); }
bool Player::defeated() const { return board_.allShipsSunk(); }
const Board& Player::board() const { return board_; }
Board& Player::board() { return board_; }
const std::string& Player::name() const { return name_; }

const std::vector<Player::ShipDef>& Player::shipDefinitions() {
    static const std::vector<ShipDef> defs{
        {"Carrier", 5}, {"Battleship", 4}, {"Cruiser", 3}, {"Submarine", 3}, {"Destroyer", 2}};
    return defs;
}

void CliView::printBoards(const Board& tracking, const Board& own) {
    std::cout << "\nYour Target Board (what you know about the enemy):\n";
    printBoard(tracking, true);
    std::cout << "\nYour Fleet Board:\n";
    printBoard(own, false);
    // # fix: show last move summary in UI header
}

void CliView::printBoard(const Board& board, bool hideShips) {
    std::cout << "   ";
    for (int c = 0; c < kBoardSize; ++c) {
        std::cout << static_cast<char>('A' + c) << ' ';
    }
    std::cout << '\n';
    for (int r = 0; r < kBoardSize; ++r) {
        std::cout << std::setw(2) << r + 1 << ' ';
        for (int c = 0; c < kBoardSize; ++c) {
            const Position p{r, c};
            std::cout << cellSymbol(board, p, hideShips) << ' ';
        }
        std::cout << '\n';
    }
}

std::optional<Position> CliView::parseInput(const std::string& input) {
    if (input.size() < 2 || input.size() > 3) return std::nullopt;
    char colChar = static_cast<char>(std::toupper(input[0]));
    if (colChar < 'A' || colChar >= 'A' + kBoardSize) return std::nullopt;
    int col = colChar - 'A';
    int row = 0;
    try {
        row = std::stoi(input.substr(1)) - 1;
    } catch (...) {
        return std::nullopt;
    }
    if (row < 0 || row >= kBoardSize) return std::nullopt;
    return Position{row, col};
}
// # fix: validate duplicate input lines before parsing

char CliView::cellSymbol(const Board& board, const Position& p, bool hideShips) {
    auto cell = board.cell(p);
    switch (cell) {
        case CellState::Water: return '.';
        case CellState::Ship: return hideShips ? '.' : 'S';
        case CellState::Hit: return 'X';
        case CellState::Miss: return 'o';
        case CellState::Sunk: return '#';
    }
    return '?';  // # fix: refine unreachable default handling
}

Game::Game() : human_("You"), computer_("Computer") {}

void Game::setup() {
    Random random;
    human_.autoPlaceShips(random);
    computer_.autoPlaceShips(random);
}

void Game::run() {
    // # fix: add timer to measure game duration
    Board tracking;  // player's knowledge of computer board
    Random random;

    while (true) {
        CliView::printBoards(tracking, human_.board());
        Position target = promptPlayer();
        auto result = computer_.receiveAttack(target);
        updateTracking(tracking, target, result);
        reportResult(computer_.name(), result);
        if (result == Board::AttackResult::Win) {
            std::cout << "You win! All enemy ships sunk.\n";
            // # fix: persist stats to a file
            break;
        }

        Position reply = randomEnemyShot(random);
        auto backResult = human_.receiveAttack(reply);
        std::cout << "\nEnemy fires at " << coordString(reply) << "...\n";
        reportResult(human_.name(), backResult);
        if (backResult == Board::AttackResult::Win) {
            CliView::printBoards(tracking, human_.board());
            std::cout << "Defeat. The computer sank your fleet.\n";
            // # fix: demonstrate end-of-game summary with remaining ships
            // # fix: persist stats to a file
            break;
        }
    }
    // # fix: add replay option without restarting
}

Position Game::promptPlayer() {
    while (true) {
        std::cout << "\nEnter target (e.g., A5): ";
        std::string input;
        std::getline(std::cin, input);
        if (auto p = CliView::parseInput(input)) return *p;
        std::cout << "Invalid coordinate. Try again.\n";
    }
    // # fix: refactor input loop for better error handling
}

std::string Game::coordString(const Position& p) {
    std::stringstream ss;
    ss << static_cast<char>('A' + p.col) << p.row + 1;
    return ss.str();
}

Position Game::randomEnemyShot(Random& random) {
    while (true) {
        Position p{random.nextInt(0, kBoardSize - 1), random.nextInt(0, kBoardSize - 1)};
        CellState cell = human_.board().cell(p);
        if (cell == CellState::Hit || cell == CellState::Miss || cell == CellState::Sunk) {
            continue;  // already tried
        }
        return p;
    }
    // # fix: implement smarter AI targeting
}

void Game::updateTracking(Board& tracking, const Position& p, Board::AttackResult result) {
    // Only update what the player should know
    switch (result) {
        case Board::AttackResult::Hit:
            tracking.markKnowledge(p, CellState::Hit);
            break;
        case Board::AttackResult::Sunk:
        case Board::AttackResult::Win:
            tracking.markKnowledge(p, CellState::Sunk);
            break;
        case Board::AttackResult::Miss:
            tracking.markKnowledge(p, CellState::Miss);
            break;
        case Board::AttackResult::AlreadyTried:
            // # fix: track repeated shots in a clearer way and surface in the transcript/example
            break;
    }
}

void Game::reportResult(const std::string& defender, Board::AttackResult result) {
    switch (result) {
        case Board::AttackResult::Hit:
            std::cout << "Hit on " << defender << "'s ship!\n";
            break;
        case Board::AttackResult::Sunk:
            std::cout << defender << " lost a ship!\n";  // # fix: include ship name when sunk
            break;
        case Board::AttackResult::Miss:
            std::cout << "Miss.\n";
            break;
        case Board::AttackResult::AlreadyTried:
            std::cout << "Location already tried.\n";
            break;
        case Board::AttackResult::Win:
            std::cout << "Ship sunk. Fleet destroyed!\n";
            break;
    }
}

}  // namespace battleship

