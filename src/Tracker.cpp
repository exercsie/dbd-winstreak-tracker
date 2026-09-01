#include "Tracker.hpp"

#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <print>
#include <fstream>
#include <algorithm>

std::ifstream Tracker::fileCreator() {
    if(!std::filesystem::exists(dbdWinTrackerDirectory)) {
        std::filesystem::create_directories(dbdWinTrackerDirectory);
    }

    // if file doesn't exist create a file and populate
    if(!std::filesystem::exists(dbdWinTrackerFile)) {
        std::ofstream createFile(dbdWinTrackerFile);
        if(!createFile) {
            throw std::runtime_error("Failed to create dbd win tracker file!");
        }

        populateFile(createFile);
    }

    // return the file after trying to open
    std::ifstream winInfoFile(dbdWinTrackerFile);
    if(!winInfoFile) {
        throw std::runtime_error("Cannot open dbd win tracker file!");
    }

    return winInfoFile;
}

void Tracker::populateFile(std::ofstream& populateFile) noexcept {
    populateFile << "KILLER    |||    WINS\n";
    populateFile << "THE-DEATHSLINGER | 0\n";
    populateFile << "THE-CLOWN | 0\n";
    populateFile << "THE-ARTIST | 0\n";
    populateFile << "THE-DEMOGORGEN | 0\n";
    populateFile << "THE-GOOD-GUY | 0\n";
    populateFile << "THE-GHOST-FACE | 0\n";
    populateFile << "THE-PIG | 0\n";
    populateFile << "THE-NEMESIS | 0\n";
    populateFile << "THE-BLIGHT | 0\n";
    populateFile << "THE-DOCTOR | 0\n";
    populateFile << "THE-FIRST | 0\n";
    populateFile << "THE-LEGION | 0\n";
    populateFile << "THE-TERRIFIER | 0\n";
    populateFile << "THE-TWINS | 0\n";
    populateFile << "THE-SKULL-MERCHANT | 0\n";
    populateFile << "THE-NURSE | 0\n";
    populateFile << "THE-JUDGEMENT | 0\n";
    populateFile << "THE-CANNIBAL | 0\n";
    populateFile << "THE-KNIGHT | 0\n";
    populateFile << "THE-TRICKSTER | 0\n";
    populateFile << "THE-HUNTRESS | 0\n";
    populateFile << "THE-LICH | 0\n";
    populateFile << "THE-GHOUL | 0\n";
    populateFile << "THE-HAG | 0\n";
    populateFile << "THE-MASTERMIND | 0\n";
    populateFile << "THE-UNKNOWN | 0\n";
    populateFile << "THE-NIGHTMARE | 0\n";
    populateFile << "THE-ONRYO | 0\n";
    populateFile << "THE-ONI | 0\n";
    populateFile << "THE-HILLBILLY | 0\n";
    populateFile << "THE-WRAITH | 0\n";
    populateFile << "THE-KRASUE | 0\n";
    populateFile << "THE-TRAPPER | 0\n";
    populateFile << "THE-XENOMORPH | 0\n";
    populateFile << "THE-SLASHER | 0\n";
    populateFile << "THE-CENOBITE | 0\n";
    populateFile << "THE-DREDGE | 0\n";
    populateFile << "THE-DARK-LORD | 0\n";
    populateFile << "THE-ANIMATRONIC | 0\n";
    populateFile << "THE-SPIRIT | 0\n";
    populateFile << "THE-SHAPE | 0\n";
    populateFile << "THE-PLAGUE | 0\n";
    populateFile << "THE-EXECUTIONER | 0\n";
    populateFile << "THE-SINGULARITY | 0\n";
}

void Tracker::buildKillerWinMap() {
    std::string stream;
    std::ifstream winInfoFile;
    winInfoFile = fileCreator();

    // skip "killer ||| wins" title
    std::getline(winInfoFile, stream);

    while(std::getline(winInfoFile, stream)) {
        const std::size_t delimiter = stream.find('|');

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
    std::ofstream trackerFile(dbdWinTrackerFile);

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