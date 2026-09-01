#include "Tracker.hpp"

#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <print>
#include <fstream>
#include <algorithm>

void Tracker::buildKillerWinMap() {
    std::string stream;
    std::ifstream trackerFile(pathToKillerWinTracker);

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
    
    // update wins
    wins = tracker.at(killer);
}

void Tracker::mapUpdater(std::unordered_map<std::string, std::uint16_t>& tracker) noexcept {
    tracker[killer] = wins;
}

void Tracker::winstreakCounter() noexcept {
    std::string enter;
    std::println("[CONSOLE] Hit enter to add one to {}'s winstreak, type 0 to exit", killer);
    std::println("[CONSOLE] Wins: {}", wins);
    while(true) {
        std::getline(std::cin, enter);
        if(enter.empty()) {
            ++wins;
            std::println("[CONSOLE] Wins: {}", wins);
        } else {
            break;
        }
    }

    mapUpdater(tracker);
    updateFile();
    std::println("[CONSOLE] {}'s winstreak is now {}", killer, wins);
}

void Tracker::resetWinstreak() noexcept {
    if(wins == 0) {
        std::println(std::cerr, "[ERROR] {}'s wins are already at 0!", killer);
        return;
    }
    
    char choice;
    while(true) {
        std::print("[CONSOLE] Are you sure you want to reset {}'s winstreak? [Y/n] ", killer);
        std::cin >> choice;
        if(std::cin.fail()) {
            std::cin.clear();
            std::println(std::cerr, "[ERROR] Please enter [Y/n]");
            continue;
        }
        
        choice = std::tolower(choice);
        if(choice == 'n') {
            std::println("[CONSOLE] {}'s winstreak reset avoided successfully", killer);
            return;
        }
        
        if(choice == 'y') {
            wins = 0;
            std::println("[CONSOLE] {}'s winstreak has been set to {}", killer, wins);
            
            mapUpdater(tracker);
            updateFile();
            return;
        }
        
        std::println(std::cerr, "[ERROR] Please enter [Y/n]");
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Tracker::updateFile() {
    std::ofstream trackerFile(pathToKillerWinTracker);

    if(!trackerFile) {
        throw std::runtime_error("Cannot open killer_win_info.txt");
    }

    trackerFile << "KILLER    |||    WINS\n";
    for(const auto& [killer, wins] : tracker) {
        trackerFile << killer << " | " << wins << '\n';
    }
}

void Tracker::specifyKillerWins(std::uint16_t w) noexcept {
    wins = w;
    mapUpdater(tracker);
    updateFile();
    std::println("[CONSOLE] {}'s winstreak has been set to {}", killer, wins);
}

void Tracker::displayKillerWinstreak(const std::string& killerName) const noexcept {
    for(const auto& [killer, wins] : tracker) {
        if(killer == killerName) {
            std::println("[CONSOLE] Killer: {}\n[CONSOLE] Wins: {}", killer, wins);
        }
    }
}

void Tracker::displayAllKillerWinstreaks() const noexcept {
    for(const auto& [killer, wins] : tracker) {
        std::println("[CONSOLE] Killer: {}\n[CONSOLE] Wins: {}", killer, wins);
    }
}

void Tracker::displayKillerWinstreaksInReferenceToN(const int n) const noexcept {
    bool notFound = true;
    for(const auto& [killer, wins] : tracker) {
        if(wins >= n) {
            std::println("[CONSOLE] Killer: {}\n[CONSOLE] Wins: {}", killer, wins);
            notFound = false;
        }
    }

    if(notFound) {
        std::println("[CONSOLE] No results found!");
    }
} 