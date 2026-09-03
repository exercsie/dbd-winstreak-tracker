#include "Tracker.hpp"

#include <unordered_map>
#include <string>
#include <cstdint>
#include <print>
#include <iostream>
#include <limits>

bool inputHandling(const int choice, const std::uint16_t lowerBound = 0, const std::uint16_t upperBound = 65535) {
    if(std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::println(std::cerr, "[ERROR] Please enter a number!");
        return false;
    }

    if(choice < lowerBound || choice > upperBound) {
        std::cin.clear();
        std::println(std::cerr, "[ERROR] Please enter a number between {} and {}", lowerBound, upperBound);
        return false;
    }

    return true;
}

int main() {
    constexpr std::uint16_t lowerBound{1};
    constexpr std::uint16_t upperBound{8};
    std::string killer;
    int choice;
    Tracker t;

    std::println("[CONSOLE] Welcome to DBD winstreak tracker.");
    while(true) {
        bool isChoiceNewKiller = false;
        std::print("[CONSOLE] Enter your killer: ");
        std::getline(std::cin, killer);
        // replace every space with a hyphon to match file
        for(std::uint16_t i{}; i < killer.size(); ++i) {
            if(killer[i] == ' ') {
                killer[i] = '-';
            }
        }
        
        // convert killer's name to uppercase for ../Files/killer_win_info.txt
        std::transform(killer.begin(), killer.end(), killer.begin(), ::toupper);
        
        // check entered killer is actually a killer
        t.setKiller(killer);
        t.buildKillerWinMap();

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
            std::println("[CONSOLE] 1 - Start counting winstreak");
            std::println("[CONSOLE] 2 - View winstreak data");
            std::println("[CONSOLE] 3 - Reset winstreak");
            std::println("[CONSOLE] 4 - Set amount of wins");
            std::println("[CONSOLE] 5 - Set personal best");
            std::println("[CONSOLE] 6 - View all killer's winstreaks");
            std::println("[CONSOLE] 7 - View all killers with a winstreak >= a number");
            std::println("[CONSOLE] 8 - Choose a new killer");
            std::print("[CONSOLE] Choose an option: ");
            std::cin >> choice;
            std::println("---------------------------------------");

            if(!inputHandling(choice, lowerBound, upperBound)) {
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear stream 

            switch(choice) {
                case 1: {
                    t.winstreakCounter();
                    break;
                }

                case 2: {
                    t.displayKillerWinstreak(killer);
                    break;
                }

                case 3: {
                    t.resetWinstreak();
                    break;
                }

                case 4: {
                    bool didAvoidSpecification{false};
                    while(true) {
                        std::print("[CONSOLE] Enter number of wins to set {}'s winstreak to (type -1 to go back): ", killer);
                        std::cin >> choice;
                        if(choice == -1) {
                            std::println("[CONSOLE] {}'s winstreak specification avoided successfully", killer);
                            didAvoidSpecification = true;
                            break;
                        }

                        if(!inputHandling(choice)) {
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

                case 5: {
                    bool didAvoidSpecification{false};
                    while(true) {
                        std::print("[CONSOLE] Enter a to set {}'s personal best to (type -1 to go back): ", killer);
                        std::cin >> choice;
                        if(choice == -1) {
                            std::println("[CONSOLE] {}'s personal best specification avoided successfully", killer);
                            didAvoidSpecification = true;
                            break;
                        }

                        if(!inputHandling(choice)) {
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

                case 6: {
                    t.displayAllKillerWinstreaks();
                    break;
                }

                case 7: {
                    while(true) {
                        std::print("[CONSOLE] Enter amount of wins to search for (type -1 to go back): ");
                        std::cin >> choice;
                        if(choice == -1) {
                            break;
                        }

                        if(!inputHandling(choice)) {
                            continue;
                        }

                        if(choice == 0) {
                            t.displayAllKillerWinstreaks();
                        }

                        t.displayKillerWinstreaksInReferenceToN(choice);
                    }

                    break;
                }

                case 8: {
                    isChoiceNewKiller = true;
                    break;
                }
            }

        }
    }


}