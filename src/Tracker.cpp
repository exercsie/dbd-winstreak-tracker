#include "Tracker.hpp"

#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <print>
#include <fstream>
#include <algorithm>

void Tracker::displayKillerWinstreak(const std::string& killerName) const {
    for(const auto& [killer, wins] : tracker) {
        if(killer == killerName) {
            std::println("Killer: {}\nWins: {}", killer, wins);
        }
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
    tracker[killer] = wins;
}

void Tracker::resetWinstreak() {
    if(wins == 0) {
        throw std::runtime_error("Cannot call resetWinstreak() when wins are already 0!");
    }

    wins = 0;
    std::println("{}'s winstreak has been set to {}", killer, wins);

    mapUpdater(tracker);
}

void Tracker::buildKillerWinMap() {
    std::string stream;
    std::ifstream trackerFile("../Files/killer_win_info.txt");

    if(!trackerFile) {
        throw std::runtime_error("Cannot open killer_win_info.txt");
    }

    // skip "killer ||| wins" title
    std::getline(trackerFile, stream);

    while(std::getline(trackerFile, stream)) {
        const std::uint16_t delimiter = stream.find('|');

        // reach end of line
        if(delimiter == std::string::npos) {
            continue;
        }

        std::string killer = stream.substr(0, delimiter);
        std::uint16_t wins = std::stoi(stream.substr(delimiter + 1));

        if(!killer.empty() && killer.back() == ' ') {
            killer.pop_back();
        }

        tracker[killer] = wins;
    }
}
