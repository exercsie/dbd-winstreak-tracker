#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <filesystem>

class Tracker {
public:
    // constructors
    explicit Tracker(const std::string& k, std::uint16_t w) : killer(k), wins(w) {};
    explicit Tracker(const std::string& k) : killer(k), wins(0) {};
    explicit Tracker() = default;

    // destructor
    ~Tracker() noexcept = default;
    
    // file
    [[nodiscard]] std::ifstream fileCreator();
    void populateFile(std::ofstream&) noexcept;

    // updaters
    void buildKillerWinMap();
    void mapUpdater(std::unordered_map<std::string, std::uint16_t>&) noexcept;
    void winstreakCounter() noexcept;
    void resetWinstreak() noexcept;
    void updateFile();
    void specifyKillerWins(std::uint16_t w) noexcept;
    void setKiller(const std::string& k) { killer = k; }

    // display
    void displayKillerWinstreak(const std::string& killerName) const noexcept;
    void displayAllKillerWinstreaks() const noexcept;
    void displayKillerWinstreaksInReferenceToN(const int n) const noexcept;

    // checkers
    [[nodiscard]] bool isValidKiller() const { return tracker.contains(killer); }

    // getters
    [[nodiscard]] std::unordered_map<std::string, std::uint16_t> getMap() const noexcept { return tracker; }

private:
    const std::filesystem::path dbdWinTrackerDirectory = std::filesystem::path(std::getenv("HOME")) / ".config/tracker";
    const std::filesystem::path dbdWinTrackerFile = dbdWinTrackerDirectory / "killer_win_info.txt";
    std::string killer;
    std::uint16_t wins{};
    std::unordered_map<std::string, std::uint16_t> tracker;
};