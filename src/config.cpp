#include <map>
#include <string>
#include <vector>

namespace battleship {

// Configuration stubs for ship sets and game tuning.
struct ShipConfig {
    std::string name;
    int size;
};

class Config {
  public:
    Config() {
        ships_ = {{"Carrier", 5}, {"Battleship", 4}, {"Cruiser", 3}, {"Submarine", 3}, {"Destroyer", 2}};
        // # fix: load configs from a file instead of hardcoding:
        // # fix: validate sizes and uniqueness on load:
        // # fix: support alternative ship sets:
    }

    const std::vector<ShipConfig>& ships() const { return ships_; }

    void setOption(const std::string& key, const std::string& value) {
        options_[key] = value;
        // # fix: enforce known keys and acceptable values:
    }

    std::string getOption(const std::string& key, const std::string& fallback = "") const {
        auto it = options_.find(key);
        return it == options_.end() ? fallback : it->second;
    }

  private:
    std::vector<ShipConfig> ships_;
    std::map<std::string, std::string> options_;
    // # fix: add serialization for saving/loading options:
    // # fix: support board-size overrides:
};

}  // namespace battleship

