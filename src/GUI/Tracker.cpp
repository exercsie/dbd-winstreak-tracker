#include "Tracker.hpp"

#include <algorithm>         // std::transform
#include <cctype>            // ::toupper
#include <cstddef>           // std::size_t
#include <cstdint>           // std::uint16_t
#include <expected>          // std::expected, std::unexpected
#include <filesystem>        // std::filesystem::exists, std::filesystem::create_directories
#include <format>            // std::format
#include <fstream>           // std::ifstream, std::ofstream
#include <stdexcept>         // std::runtime_error
#include <string>            // std::string, std::getline, std::stoi

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
        if(delimiter == std::string::npos) { // reach end of line
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

std::string Tracker::killerNormalisation(std::string killer) {
    // Replace every space with a -
    for(std::uint16_t i{}; i < killer.size(); ++i) {
        if(killer[i] == ' ') {
            killer[i] = '-';
        }
    }

    // Convert all chars to uppercase
    std::transform(killer.begin(), killer.end(), killer.begin(), ::toupper);

    // Killer aliases
    const std::unordered_map<std::string, std::string> aliases {
        {"BUBBA", "THE-CANNIBAL"},
        {"LEATHERFACE", "THE-CANNIBAL"},
        {"LEATHER-FACE", "THE-CANNIBAL"},
        {"BILLY", "THE-HILLBILLY"},
        {"DEMO", "THE-DEMOGORGEN"},
        {"WESKER", "THE-MASTERMIND"},
        {"MYERS", "THE-SHAPE"},
        {"MICHAEL-MYERS", "THE-SHAPE"},
        {"DOC", "THE-DOCTOR"},
        {"FREDDY", "THE-NIGHTMARE"},
        {"FREDDY-KRUEGER", "THE-NIGHTMARE"},
        {"GHOSTFACE", "THE-GHOST-FACE"},
        {"SLINGER", "THE-DEATHSLINGER"},
        {"PYRAMID-HEAD", "THE-EXECUTIONER"},
        {"PYRAMIDHEAD", "THE-EXECUTIONER"},
        {"PINHEAD", "THE-CENOBITE"},
        {"PIN-HEAD", "THE-CENOBITE"},
        {"SADAKO", "THE-ONRYO"},
        {"XENO", "THE-XENOMORPH"},
        {"CHUCKY", "THE-GOOD-GUY"},
        {"VECNA", "THE-LICH"},
        {"DRACULA", "THE-DARK-LORD"},
        {"DRAC", "THE-DARK-LORD"},
        {"KEN", "THE-GHOUL"},
        {"KEN-KANEKI", "THE-GHOUL"},
        {"SPRINGTRAP", "THE-ANIMATRONIC"}
    };

    if(const auto it = aliases.find(killer); it != aliases.end()) {
        killer = it->second;
    } else if(!killer.starts_with("THE-")) {
        killer = std::format("THE-{}", killer);
    }

    return killer;
}

void Tracker::incrementWins() {
    ++d.wins;
    if(d.personalBest < d.wins) {
        d.personalBest = d.wins;
    }

    mapUpdater();
    updateFile();
}

std::expected<void, std::string> Tracker::decrementWins() {
    if(d.wins == 0) {
        return std::unexpected("Winstreak cannot go below 0!");
    }

    if(d.personalBest == d.wins) {
        --d.personalBest;
    }

    --d.wins;
    mapUpdater();
    updateFile();
    return {};
}

std::expected<void, std::string> Tracker::resetWinstreak() {
    if(d.wins == 0) {
        return std::unexpected(std::format("{}'s winstreak is already at 0!", killer));
    }
    
    d.wins = 0;
    mapUpdater();
    updateFile();
    return {};
}

std::expected<void, std::string> Tracker::resetPersonalBest() {
    if(d.personalBest == 0) {
        return std::unexpected(std::format("{}'s personal best is already at 0!", killer));
    }
    
    d.wins = 0;
    d.personalBest = 0;
    mapUpdater();
    updateFile();
    return {};
}


void Tracker::specifyKillerWins(std::uint16_t w) {
    if(d.personalBest < w) {
        d.wins = w;
        d.personalBest = w;
        mapUpdater();
        updateFile();
        return;
    }

    d.wins = w;
    mapUpdater();
    updateFile();
}

void Tracker::setPersonalBest(std::uint16_t pb) {
    if(d.wins > pb) {
        d.wins = pb;
        d.personalBest = pb;
        mapUpdater();
        updateFile();
        return;
    }

    d.personalBest = pb;
    mapUpdater();
    updateFile();
}

/*void Tracker::displaySpecificKillerStats(const std::string& killerName) const noexcept {
    for(const auto& [killer, data] : tracker) {
        if(killer == killerName) {
            std::println("---------------------------------------");
            std::println("[CONSOLE] Killer: {}\n[CONSOLE] Wins: {}\n[CONSOLE] PB: {}", killer, data.wins, data.personalBest);
        }
    }
}

void Tracker::displayAllKillerStats() const noexcept {
    for(const auto& [killer, data] : tracker) {
        std::println("---------------------------------------");
        std::println("[CONSOLE] Killer: {}\n[CONSOLE] Wins: {}\n[CONSOLE] PB: {}", killer, data.wins, data.personalBest);
    }
}

void Tracker::displayKillerWinstreaksInReferenceToN(const int n) const noexcept {
    bool notFound = true;
    for(const auto& [killer, data] : tracker) {
        if(data.wins >= n) {
            std::println("---------------------------------------");
            std::println("[CONSOLE] Killer: {}\n[CONSOLE] Wins: {}", killer, data.wins);
            notFound = false;
        }
    }

    std::println("---------------------------------------");

    if(notFound) {
        std::println("---------------------------------------");
        std::println("[CONSOLE] No results found!");
    }
} 

void Tracker::displayKillerPersonalBestsInReferenceToN(const int n) const noexcept {
    bool notFound = true;
    for(const auto& [killer, data] : tracker) {
        if(data.personalBest >= n) {
            std::println("---------------------------------------");
            std::println("[CONSOLE] Killer: {}\n[CONSOLE] PB: {}", killer, data.personalBest);
            notFound = false;
        }
    }
    
    std::println("---------------------------------------");

    if(notFound) {
        std::println("---------------------------------------");
        std::println("[CONSOLE] No results found!");
    }
}*/