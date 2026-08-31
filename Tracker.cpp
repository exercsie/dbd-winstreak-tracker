#include "Tracker.hpp"

#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <print>

void Tracker::displayMap() const {
    for(const auto& [killer, wins] : tracker) {
        std::println("Killer: {}\nWins: {}", killer, wins);
    }
}

void Tracker::winstreakCounter() noexcept {
    std::string enter;
    std::println("Hit enter to add one to the win, type 0 to exit");
    std::println("Wins: {}", wins);
    while(true) {
        std::getline(std::cin, enter);
        if(enter.empty()) {
            ++wins;
            std::println("Wins: {}", wins);
        } else {
            break;
        }
    }

    mapUpdater(tracker);
}

void Tracker::mapUpdater(std::unordered_map<std::string, std::uint16_t>& tracker) noexcept {
    tracker = {
        { killer, wins }
    };
}

void Tracker::resetWinstreak() {
    if(wins == 0) {
        throw std::runtime_error("Cannot call resetWinstreak() when wins are already 0!");
    }

    wins = 0;
    if(wins == 0) {
        std::println("{}'s winstreak has been set to {}", killer, wins);
    }

    mapUpdater(tracker);
}


