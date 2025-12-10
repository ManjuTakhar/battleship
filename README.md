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


## Strategy Basics
- Fire in a checkerboard pattern to cover cells efficiently.
- After a hit, probe orthogonally to find ship orientation.
- Track sunk ships to avoid redundant guesses.

## Improvements (selected # fix)
- Smarter AI, manual placement, reproducible seeds.
- Configurable board sizes and ship sets.
- Colorized output, clearer repeated-shot feedback, richer examples.

## Planned Fixes to be done
- Config loading/persistence: load from a local `battleship.config` (simple `key=value` or JSON). On startup, read the file, validate, and fall back to defaults; expose `saveToFile()` to persist changes.
- Config validation: enforce board size in a safe range (e.g., 5–20), ship sizes >0 and <= board size, unique/non-empty ship names, and whitelist option keys.
- Board size overrides: allow board size to be set in config; reject configs where ships don’t fit; propagate board size to game setup.
- Ship sets: support multiple named ship presets; select via config key; default to the classic fleet.
- Serialization APIs: add `loadFromFile(path)` and `saveToFile(path)` on `Config`; handle missing/invalid files gracefully.
- Integration: have `Player` pull ship definitions from `Config::ships()` and make the board size driven by config instead of a fixed constant at startup.
- Deterministic randomness: allow injecting a seed (CLI/env/config) into `Random` so games can be replayed exactly.
- Hit tracking: switch `Ship` hit tracking to a `std::set` for O(log n) lookup and duplicate prevention.
- Ship placement options: support manual placement flow for the human player; keep auto-placement for the computer.
- Board rendering: show last-move summary in the CLI header; keep symbols aligned with legend.
- Input parsing: trim whitespace, require A–J plus 1–10, reject leading zeros/trailing chars; keep re-prompt loop.
- Attack feedback: include ship names when sunk; improve “already tried” messaging on repeated shots.
- Game loop extras: add game duration timing; offer replay without restart; persist win/loss stats to a file.
- AI targeting: add smarter follow-up on hits (orientation/adjacent targeting) instead of pure random.
- Knowledge board rules: only mark public states (hit/miss/sunk) and ignore invalid states.
- Placement spacing: enforce no-touch/no-overlap (including diagonals) when placing ships.

## Notes
- Ships are placed randomly with collision checks.
- Code is organized into small classes: `Position`, `Ship`, `Board`, `Player`, `CliView`, `Game`.
