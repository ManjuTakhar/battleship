# Battleship (CLI, C++17)

Interactive command-line Battleship written in modern C++. You and the computer each get a 10x10 grid with a standard fleet placed randomly (no overlaps). On your turn, you enter coordinates like `A5` to fire at the opponent. Hits and misses are shown immediately, and when every segment of a ship is hit it is marked sunk. You win by sinking all enemy ships before yours are sunk; the computer fires back each turn with simple random targeting. The interface shows two boards after each move: your knowledge of the enemy and your own fleet’s status.

## Build
- Ensure a C++17 compiler is available (e.g., `g++`).
- From the project root:
  - `g++ -std=c++17 -O2 -o battleship src/main.cpp src/board.cpp src/game.cpp src/render.cpp src/config.cpp`

## Play
- Run `./battleship`.
- Enter shots as `A5`, `J10`, etc. Rows are 1-10, columns A-J.
- After each turn, you see:
  - Target board (your knowledge of the opponent)
  - Fleet board (your ships, hits, and misses)
- Game ends when either fleet is fully sunk.

## Input Rules
- Columns A-J (case-insensitive), rows 1-10. No commas/spaces needed (e.g., `B7`, `j10`).
- Invalid entries are rejected; you’ll be prompted again.
- # fix: clarify behavior for duplicate/whitespace inputs.


## Strategy Basics
- Fire in a checkerboard pattern to cover cells efficiently.
- After a hit, probe orthogonally to find ship orientation.
- Track sunk ships to avoid redundant guesses.

## Improvements (selected # fix)
- Smarter AI, manual placement, reproducible seeds.
- Configurable board sizes and ship sets.
- Colorized output, clearer repeated-shot feedback, richer examples.

## Notes
- Ships are placed randomly with collision checks.
- Code is organized into small classes: `Position`, `Ship`, `Board`, `Player`, `CliView`, `Game`.
