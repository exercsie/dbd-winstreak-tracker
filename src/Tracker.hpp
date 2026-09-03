#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <filesystem>

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
    // constructors
    explicit Tracker(const std::string& k, std::uint16_t w) : killer(k), d{w, 0} {};
    explicit Tracker(const std::string& k) : killer(k), d{0, 0} {};
    explicit Tracker() = default;

    // destructor
    ~Tracker() noexcept = default;
    
    // file
    [[nodiscard]] std::ifstream fileCreator();
    void populateFile(std::ofstream&) noexcept;

    // updaters
    void buildKillerWinMap();
    void mapUpdater() noexcept;
    void winstreakCounter() noexcept;
    void resetWinstreak() noexcept;
    void updateFile();
    void specifyKillerWins(std::uint16_t w) noexcept;
    void setPersonalBest(std::uint16_t pb) noexcept;
    void setKiller(const std::string& k) { killer = k; }

    // display
    void displayKillerWinstreak(const std::string& killerName) const noexcept;
    void displayAllKillerWinstreaks() const noexcept;
    void displayKillerWinstreaksInReferenceToN(const int n) const noexcept;

    // checkers
    [[nodiscard]] bool isValidKiller() const { return tracker.contains(killer); }

    // getters
    [[nodiscard]] std::unordered_map<std::string, data> getMap() const noexcept { return tracker; }

};