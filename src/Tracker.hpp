#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>

class Tracker {
public:
    explicit Tracker(const std::string& k, std::uint16_t w) : killer(k), wins(w) {};
    explicit Tracker(std::string& k) : killer(k), wins(0) {};
    explicit Tracker() = default;

    void buildKillerWinMap();
    void displayKillerWinstreak(const std::string& killerName) const;
    void winstreakCounter() noexcept;
    void mapUpdater(std::unordered_map<std::string, std::uint16_t>&) noexcept;
    void resetWinstreak();
    void setKiller(const std::string& k) { killer = k; }
    bool isValidKiller() const { return tracker.contains(killer); }

    // getters
    std::unordered_map<std::string, std::uint16_t> getMap() const noexcept { return tracker; }

private:
    std::string killer;
    std::uint16_t wins{};
    std::unordered_map<std::string, std::uint16_t> tracker;

};