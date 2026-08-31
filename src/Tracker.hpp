#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>

class Tracker {
public:
    explicit Tracker(const std::string& k, std::uint16_t w) : killer(k), wins(w) {};
    explicit Tracker(std::string k) : killer(k), wins(0) {};

    void displayMap() const;
    void winstreakCounter() noexcept;
    void mapUpdater(std::unordered_map<std::string, std::uint16_t>&) noexcept;
    void resetWinstreak();
private:
    std::string killer;
    std::uint16_t wins{};
    std::unordered_map<std::string, std::uint16_t> tracker =  { {killer, wins} };
};