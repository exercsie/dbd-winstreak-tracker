#pragma once

#include <cstdint>          // std::uint16_t
#include <cstdlib>          // std::getenv
#include <expected>         // std::expected, std::unexpected
#include <filesystem>       // std::filesystem::path
#include <fstream>          // std::ofstream, std::ifstream
#include <string>           // std::string
#include <unordered_map>    // std::unordered_map

class Tracker {
public:
    // Data
    struct data {
        std::uint16_t wins{};
        std::uint16_t personalBest{};
    };

    // Constructors
    explicit Tracker(const std::string& k, std::uint16_t w) : killer(k), d{w, 0} {};
    explicit Tracker(const std::string& k) : killer(k), d{0, 0} {};
    explicit Tracker() = default;

    // Destructor
    ~Tracker() noexcept = default;

    // File logic
    [[nodiscard]] std::ifstream fileCreator();
    void populateFile(std::ofstream&) noexcept;
    void buildKillerWinMap();
    void mapUpdater() noexcept;
    void updateFile();

    // Updaters
    [[nodiscard]] static std::string killerNormalisation(std::string k);
    void incrementWins();
    [[nodiscard]] std::expected<void, std::string> decrementWins();
    [[nodiscard]] std::expected<void, std::string> resetWinstreak();
    [[nodiscard]] std::expected<void, std::string> resetPersonalBest();
    [[nodiscard]] std::expected<void, std::string> setWins(int w);
    [[nodiscard]] std::expected<void, std::string> setPersonalBest(int pb);
    void setKiller(const std::string& k) { killer = k; }

    // Checker
    [[nodiscard]] bool isValidKiller() const { return tracker.contains(killer); }

    // Getters
    [[nodiscard]] std::uint16_t getWins() const noexcept { return d.wins; }
    [[nodiscard]] std::uint16_t getPersonalBest() const noexcept { return d.personalBest; }
    [[nodiscard]] const std::unordered_map<std::string, data>& getMap() const noexcept { return tracker; }
    [[nodiscard]] const std::string getKiller() const noexcept { return killer; }
private:
    const std::filesystem::path dbdWinTrackerDirectory = std::filesystem::path(std::getenv("HOME")) / ".config/tracker";
    const std::filesystem::path dbdWinTrackerFile = dbdWinTrackerDirectory / "killer_win_info.txt";
    std::string killer;
    
    std::unordered_map<std::string, data> tracker;
    data d;
};