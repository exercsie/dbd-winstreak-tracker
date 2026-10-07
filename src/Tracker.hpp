#pragma once

#include <cstdint>          // std::uint16_t
#include <cstdlib>          // std::getenv
#include <filesystem>       // std::filesystem::path
#include <fstream>          // std::ofstream, std::ifstream
#include <string>           // std::string
#include <unordered_map>    // std::unordered_map

class Tracker {
private:
    const std::filesystem::path dbdWinTrackerDirectory = std::filesystem::path(std::getenv("HOME")) / ".config/tracker";
    const std::filesystem::path dbdWinTrackerFile = dbdWinTrackerDirectory / "killer_win_info.txt";
    std::string killer;
    struct data {
        std::uint16_t wins{};
        std::uint16_t personalBest{};
    };

    std::unordered_map<std::string, data> tracker;
    data d;

public:
    // Constructors
    explicit Tracker(const std::string& k, std::uint16_t w) : killer(k), d{w, 0} {};
    explicit Tracker(const std::string& k) : killer(k), d{0, 0} {};
    explicit Tracker() = default;

    // Destructor
    ~Tracker() noexcept = default;
    
    // File logic
    [[nodiscard]] std::ifstream fileCreator();
    void populateFile(std::ofstream&) noexcept;

    // Updaters
    void buildKillerWinMap();
    void mapUpdater() noexcept;
    void winstreakCounter() noexcept;
    void resetWinstreak() noexcept;
    void resetPersonalBest() noexcept;
    void updateFile();
    void specifyKillerWins(std::uint16_t w) noexcept;
    void setPersonalBest(std::uint16_t pb) noexcept;
    void setKiller(const std::string& k) { killer = k; }

    // Display logic
    void displaySpecificKillerStats(const std::string& killerName) const noexcept;
    void displayAllKillerStats() const noexcept;
    void displayKillerWinstreaksInReferenceToN(const int n) const noexcept;
    void displayKillerPersonalBestsInReferenceToN(const int n) const noexcept;

    // Checker
    [[nodiscard]] bool isValidKiller() const { return tracker.contains(killer); }

    // Getter
    [[nodiscard]] std::unordered_map<std::string, data> getMap() const noexcept { return tracker; }
};