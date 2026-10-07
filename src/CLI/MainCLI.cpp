#include "TrackerCLI.hpp"
#include "MainCLI.hpp"

#include <algorithm>         // std::transform
#include <cctype>            // ::toupper
#include <cstdint>           // std::uint16_t
#include <expected>          // std::expected, std::unexpected
#include <format>            // std::format
#include <iostream>          // std;:cin, std::cerr, std::streamsize
#include <limits>            // std::numeric_limits, ::max
#include <print>             // std::print, std::println
#include <string>            // std::string, std::getline
#include <unordered_map>     // std::unordered_map

std::expected<void, std::string> Menu::inputHandling(const int choice, const std::uint16_t lowerBound, const std::uint16_t upperBound) {
    if(std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::unexpected("Please enter a number!");
    }

    if(choice < lowerBound || choice > upperBound) {
        return std::unexpected(std::format("Please enter a number between {} and {}", lowerBound, upperBound));
    }

    return {};
}

void Menu::killerAliases(std::string& k) noexcept {
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

    if(const auto it = aliases.find(k); it != aliases.end()) {
        k = it->second;
    } else if(!k.starts_with("THE-")) {
        k = std::format("THE-{}", k);
    }
}

int main() {
    Menu m;
    constexpr std::uint16_t lowerBound{0};
    constexpr std::uint16_t upperBound{6};
    std::string killer;
    int choice;
    Tracker t;

    std::println("[CONSOLE] Welcome to DBD winstreak tracker.");
    while(true) {
        bool isChoiceNewKiller = false;
        std::print("[CONSOLE] Enter your killer: ");
        std::getline(std::cin, killer);

        if(killer.empty()) {
            std::println(std::cerr, "[ERROR] Please enter a killer");
            continue;
        }

        // replace every space with a hyphon to match file
        for(std::uint16_t i{}; i < killer.size(); ++i) {
            if(killer[i] == ' ') {
                killer[i] = '-';
            }
        }
        
        // convert killer's name to uppercase for killer_win_info.txt
        std::transform(killer.begin(), killer.end(), killer.begin(), ::toupper);
        
        m.killerAliases(killer);
        
        t.setKiller(killer);
        t.buildKillerWinMap();
        
        // check entered killer is actually a killer
        if(!t.isValidKiller()) {
            std::println(std::cerr, "[ERROR] {} does not exist. Example usage, \"The Terrifier\".", killer);
            continue;
        }
        
        while(true) {
            if(isChoiceNewKiller) {
                break;
            }

            std::println("---------------------------------------");
            std::println("[CONSOLE] Selected killer: {}", killer);
            std::println("[CONSOLE] 0 - Exit");
            std::println("[CONSOLE] 1 - Start counting winstreak");
            std::println("[CONSOLE] 2 - View {}'s stats", killer);
            std::println("[CONSOLE] 3 - Reset stats options");
            std::println("[CONSOLE] 4 - Set stats options");
            std::println("[CONSOLE] 5 - Query stats");
            std::println("[CONSOLE] 6 - Choose a new killer");
            std::print("[CONSOLE] Choose an option: ");
            std::cin >> choice;

            const std::expected result = m.inputHandling(choice, lowerBound, upperBound);
            if(!result) {
                std::println(std::cerr, "[ERROR] {}", result.error());
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear stream 

            switch(choice) {
                case 0: {
                    return 0;
                }

                case 1: {
                    t.winstreakCounter();
                    break;
                }

                case 2: {
                    t.displaySpecificKillerStats(killer);
                    break;
                }

                case 3: {
                    while(true) {
                        std::println("---------------------------------------");
                        std::println("[CONSOLE] 1 - Reset winstreak");
                        std::println("[CONSOLE] 2 - Reset personal best");
                        std::print("[CONSOLE] Enter an option (type -1 to go back): ");
                        std::cin >> choice;
                        if(choice == -1) {
                            break;
                        }

                        const std::expected result = m.inputHandling(choice, 1, 2);
                        if(!result) {
                            std::println(std::cerr, "[ERROR] {}", result.error());
                            continue;
                        }

                        switch(choice) {
                            case 1: {
                                t.resetWinstreak();
                                break;
                            }

                            case 2: {
                                t.resetPersonalBest();
                                break;
                            }
                        }
                    }

                    break;
                }

                case 4: {
                    while(true) {
                        std::println("---------------------------------------");
                        std::println("[CONSOLE] 1 - Set winstreak");
                        std::println("[CONSOLE] 2 - Set personal best");
                        std::print("[CONSOLE] Enter an option (type -1 to go back): ");
                        std::cin >> choice;
                        if(choice == -1) {
                            break;
                        }

                        const std::expected result = m.inputHandling(choice, 1, 2);
                        if(!result) {
                            std::println(std::cerr, "[ERROR] {}", result.error());
                            continue;
                        }

                        switch(choice) {
                            case 1: {
                                bool didAvoidSpecification{false};
                                while(true) {
                                    std::print("[CONSOLE] Enter number of wins to set {}'s winstreak to (type -1 to go back): ", killer);
                                    std::cin >> choice;
                                    if(choice == -1) {
                                        std::println("[CONSOLE] {}'s winstreak specification avoided successfully", killer);
                                        didAvoidSpecification = true;
                                        break;
                                    }

                                    const std::expected result = m.inputHandling(choice);
                                    if(!result) {
                                        std::println(std::cerr, "[ERROR] {}", result.error());
                                        continue;
                                    }

                                    break;
                                }

                                if(!didAvoidSpecification) {
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                    t.specifyKillerWins(choice);
                                    break;
                                }

                                break;
                            }

                            case 2: {
                                bool didAvoidSpecification{false};
                                while(true) {
                                    std::print("[CONSOLE] Enter a number to set {}'s personal best (type -1 to go back): ", killer);
                                    std::cin >> choice;
                                    if(choice == -1) {
                                        std::println("[CONSOLE] {}'s personal best specification avoided successfully", killer);
                                        didAvoidSpecification = true;
                                        break;
                                    }

                                    const std::expected result = m.inputHandling(choice);
                                    if(!result) {
                                        std::println(std::cerr, "[ERROR] {}", result.error());
                                        continue;
                                    }

                                    break;
                                }

                                if(!didAvoidSpecification) {
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                    t.setPersonalBest(choice);
                                    break;
                                }

                                break;
                            }
                        }
                    }

                    break;
                }

                case 5: {
                    while(true) {
                        std::println("---------------------------------------");
                        std::println("[CONSOLE] 1 - View all killer's info");
                        std::println("[CONSOLE] 2 - View all killer's winstreaks >= a number");
                        std::println("[CONSOLE] 3 - View all killer's personal bests >= a number");
                        std::print("[CONSOLE] Enter an option (type -1 to go back): ");
                        std::cin >> choice;

                        if(choice == -1) {
                            break;
                        }

                        const std::expected result = m.inputHandling(choice, 1, 3);
                        if(!result) {
                            std::println(std::cerr, "[ERROR] {}", result.error());
                            continue;
                        }

                        switch(choice) {
                            case 1: {
                                t.displayAllKillerStats();
                                break;
                            }

                            case 2: {
                                while(true) {
                                    std::print("[CONSOLE] Enter amount of wins to search for (type -1 to go back): ");
                                    std::cin >> choice;
                                    if(choice == -1) {
                                        break;
                                    }

                                    const std::expected result = m.inputHandling(choice);
                                    if(!result) {
                                        std::println(std::cerr, "[ERROR] {}", result.error());
                                        continue;
                                    }

                                    t.displayKillerWinstreaksInReferenceToN(choice);
                                }

                                break;
                            }

                            case 3: {
                                while(true) {
                                    std::print("[CONSOLE] Enter a personal best to search for (type -1 to go back): ");
                                    std::cin >> choice;
                                    if(choice == -1) {
                                        break;
                                    }

                                    const std::expected result = m.inputHandling(choice);
                                    if(!result) {
                                        std::println(std::cerr, "[ERROR] {}", result.error());
                                        continue;
                                    }

                                    t.displayKillerPersonalBestsInReferenceToN(choice);
                                }

                                break;
                            }
                        }
                    }

                    break;
                }

                case 6: {
                    isChoiceNewKiller = true;
                    break;
                }
            }
        }
    }
}