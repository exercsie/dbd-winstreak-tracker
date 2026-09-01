#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>

class Tracker {
public:
    // constructors
    explicit Tracker(const std::string& k, std::uint16_t w) : killer(k), wins(w) {};
    explicit Tracker(std::string& k) : killer(k), wins(0) {};
    explicit Tracker() = default;

    // destructor
    ~Tracker() noexcept = default;
    
    // updaters
    void buildKillerWinMap();
    void mapUpdater(std::unordered_map<std::string, std::uint16_t>&) noexcept;
    void winstreakCounter() noexcept;
    void resetWinstreak();
    void setKiller(const std::string& k) { killer = k; }
    void updateFile();
    void specifyKillerWins(std::uint16_t w) noexcept;

    // display
    void displayKillerWinstreak(const std::string& killerName) const noexcept;
    void displayAllKillerWinstreaks() const noexcept;
    void displayKillerWinstreaksInReferenceToN(const int n) const noexcept;

    // checkers
    bool isValidKiller() const { return tracker.contains(killer); }

    // getters
    std::unordered_map<std::string, std::uint16_t> getMap() const noexcept { return tracker; }

private:
    std::string killer;
    const std::string pathToKillerWinTracker = {"../Files/killer_win_info.txt"};
    std::uint16_t wins{};
    std::unordered_map<std::string, std::uint16_t> tracker;
};