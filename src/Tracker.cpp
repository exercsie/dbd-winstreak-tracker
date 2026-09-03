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
    populateFile << "KILLER    |||    WINS    |||    PB\n";
    populateFile << "THE-DEATHSLINGER | 0 | 0\n";
    populateFile << "THE-CLOWN | 0 | 0\n";
    populateFile << "THE-ARTIST | 0 | 0\n";
    populateFile << "THE-DEMOGORGEN | 0 | 0\n";
    populateFile << "THE-GOOD-GUY | 0 | 0\n";
    populateFile << "THE-GHOST-FACE | 0 | 0\n";
    populateFile << "THE-PIG | 0 | 0\n";
    populateFile << "THE-NEMESIS | 0 | 0\n";
    populateFile << "THE-BLIGHT | 0 | 0\n";
    populateFile << "THE-DOCTOR | 0 | 0\n";
    populateFile << "THE-FIRST | 0 | 0\n";
    populateFile << "THE-LEGION | 0 | 0\n";
    populateFile << "THE-TERRIFIER | 0 | 0\n";
    populateFile << "THE-TWINS | 0 | 0\n";
    populateFile << "THE-SKULL-MERCHANT | 0 | 0\n";
    populateFile << "THE-NURSE | 0 | 0\n";
    populateFile << "THE-JUDGEMENT | 0 | 0\n";
    populateFile << "THE-CANNIBAL | 0 | 0\n";
    populateFile << "THE-KNIGHT | 0 | 0\n";
    populateFile << "THE-TRICKSTER | 0 | 0\n";
    populateFile << "THE-HUNTRESS | 0 | 0\n";
    populateFile << "THE-LICH | 0 | 0\n";
    populateFile << "THE-GHOUL | 0 | 0\n";
    populateFile << "THE-HAG | 0 | 0\n";
    populateFile << "THE-MASTERMIND | 0 | 0\n";
    populateFile << "THE-UNKNOWN | 0 | 0\n";
    populateFile << "THE-NIGHTMARE | 0 | 0\n";
    populateFile << "THE-ONRYO | 0 | 0\n";
    populateFile << "THE-ONI | 0 | 0\n";
    populateFile << "THE-HILLBILLY | 0 | 0\n";
    populateFile << "THE-WRAITH | 0 | 0\n";
    populateFile << "THE-KRASUE | 0 | 0\n";
    populateFile << "THE-TRAPPER | 0 | 0\n";
    populateFile << "THE-XENOMORPH | 0 | 0\n";
    populateFile << "THE-SLASHER | 0 | 0\n";
    populateFile << "THE-CENOBITE | 0 | 0\n";
    populateFile << "THE-DREDGE | 0 | 0\n";
    populateFile << "THE-DARK-LORD | 0 | 0\n";
    populateFile << "THE-ANIMATRONIC | 0 | 0\n";
    populateFile << "THE-SPIRIT | 0 | 0\n";
    populateFile << "THE-SHAPE | 0 | 0\n";
    populateFile << "THE-PLAGUE | 0 | 0\n";
    populateFile << "THE-EXECUTIONER | 0 | 0\n";
    populateFile << "THE-SINGULARITY | 0 | 0\n";
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

        const std::size_t secondDelimiter = stream.find('|', delimiter + 1);

        if(secondDelimiter == std::string::npos) {
            continue;
        }

        std::string killer = stream.substr(0, delimiter);
        std::uint16_t wins = std::stoi(stream.substr(delimiter + 1, secondDelimiter - delimiter - 1));
        std::uint16_t pb = std::stoi(stream.substr(secondDelimiter + 1));

        if(!killer.empty() && killer.back() == ' ') {
            killer.pop_back();
        }

        tracker[killer] = data{wins, pb};
    }
    
    // update data
    if(tracker.contains(this->killer)) {
        d = tracker.at(this->killer);
    }
}

void Tracker::mapUpdater() noexcept {
    tracker[killer] = d;
}

void Tracker::winstreakCounter() noexcept {
    std::string enter;
    std::println("[CONSOLE] Hit enter to add one to {}'s winstreak, type 0 to save", killer);
    std::println("[CONSOLE] Wins: {}", d.wins);
    while(true) {
        std::getline(std::cin, enter);
        if(enter.empty()) {
            ++d.wins;
            std::println("[CONSOLE] Wins: {}", d.wins);
            if(d.personalBest < d.wins) {
                d.personalBest = d.wins;
            }

            mapUpdater();
            updateFile();
        } else if(enter == "-") {
            if(d.wins == 0) {
                std::println(std::cerr, "[ERROR] Cannot decrement winstreak past 0!");
                break;
            }

            if(d.personalBest == d.wins) {
                --d.personalBest;
            }

            --d.wins;
            std::println("[CONSOLE] Wins: {}", d.wins);
            mapUpdater();
            updateFile();
        } else {
            break;
        }
    }

    std::println("[CONSOLE] {}'s winstreak is now {}", killer, d.wins);
}

void Tracker::resetWinstreak() noexcept {
    if(d.wins == 0) {
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
            d.wins = 0;
            std::println("[CONSOLE] {}'s winstreak has been set to {}", killer, d.wins);
            
            mapUpdater();
            updateFile();
            return;
        }
        
        std::println(std::cerr, "[ERROR] Please enter [Y/n]");
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Tracker::resetPersonalBest() noexcept {
    if(d.personalBest == 0) {
        std::println(std::cerr, "[ERROR] {}'s personal best is already at 0!", killer);
        return;
    }
    
    char choice;
    while(true) {
        std::print("[CONSOLE] Are you sure you want to reset {}'s personal best? [Y/n] ", killer);
        std::cin >> choice;
        if(std::cin.fail()) {
            std::cin.clear();
            std::println(std::cerr, "[ERROR] Please enter [Y/n]");
            continue;
        }
        
        choice = std::tolower(choice);
        if(choice == 'n') {
            std::println("[CONSOLE] {}'s personal best reset avoided successfully", killer);
            return;
        }
        
        if(choice == 'y') {
            d.personalBest = 0;
            std::println("[CONSOLE] {}'s personal best has been set to {}", killer, d.wins);
            
            mapUpdater();
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

    trackerFile << "KILLER    |||    WINS    |||    PB\n";
    for(const auto& [killer, data] : tracker) {
        trackerFile << killer << " | " << data.wins << " | " << data.personalBest << '\n';
    }
}

void Tracker::specifyKillerWins(std::uint16_t w) noexcept {
    if(d.personalBest < w) {
        d.wins = w;
        d.personalBest = w;
        mapUpdater();
        updateFile();
        std::println("[CONSOLE] {}'s winstreak and personal best has been set to {}", killer, w);
        return;
    }

    d.wins = w;
    mapUpdater();
    updateFile();
    std::println("[CONSOLE] {}'s winstreak has been set to {}", killer, d.wins);
}

void Tracker::setPersonalBest(std::uint16_t pb) noexcept {
    if(d.wins > pb) {
        d.wins = pb;
        d.personalBest = pb;
        mapUpdater();
        updateFile();
        std::println("[CONSOLE] {}'s personal best and winstreak has been set to: {}", killer, pb);
        return;
    }

    d.personalBest = pb;
    mapUpdater();
    updateFile();
    std::println("[CONSOLE] {}'s personal best has been set to: {}", killer, pb);
}

void Tracker::displayKillerWinstreak(const std::string& killerName) const noexcept {
    for(const auto& [killer, data] : tracker) {
        if(killer == killerName) {
            std::println("[CONSOLE] Killer: {}\n[CONSOLE] Wins: {}\n[CONSOLE] PB: {}", killer, data.wins, data.personalBest);
        }
    }
}

void Tracker::displayAllKillerInfo() const noexcept {
    for(const auto& [killer, data] : tracker) {
        std::println("[CONSOLE] Killer: {}\n[CONSOLE] Wins: {}\n[CONSOLE] PB: {}", killer, data.wins, data.personalBest);
    }
}

void Tracker::displayKillerWinstreaksInReferenceToN(const int n) const noexcept {
    bool notFound = true;
    for(const auto& [killer, data] : tracker) {
        if(data.wins >= n) {
            std::println("[CONSOLE] Killer: {}\n[CONSOLE] Wins: {}", killer, data.wins);
            notFound = false;
        }
    }

    if(notFound) {
        std::println("[CONSOLE] No results found!");
    }
} 

void Tracker::displayKillerPersonalBestsInReferenceToN(const int n) const noexcept {
    bool notFound = true;
    for(const auto& [killer, data] : tracker) {
        if(data.personalBest >= n) {
            std::println("[CONSOLE] Killer: {}\n[CONSOLE] PB: {}", killer, data.personalBest);
            notFound = false;
        }
    }

    if(notFound) {
        std::println("[CONSOLE] No results found!");
    }
}