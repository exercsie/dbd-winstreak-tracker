#include "Tracker.hpp"

#include <unordered_map>
#include <string>
#include <cstdint>
#include <print>
#include <iostream>
#include <limits>

bool inputHandling(const std::int16_t choice, const std::uint16_t lowerBound = 0, const std::uint16_t upperBound = 32767) {
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
    constexpr std::uint16_t upperBound{6};
    std::string killer;
    std::int16_t choice;
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
            std::println("[CONSOLE] Choose an option: ");
            std::println("1 - Start counting winstreak on {}", killer);
            std::println("2 - View winstreak on {}", killer);
            std::println("3 - Reset {}'s winstreak", killer);
            std::println("4 - Choose a new killer");
            std::println("5 - View all Killer's winstreaks");
            std::println("6 - Choose {}'s amount of wins", killer);
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
                    isChoiceNewKiller = true;
                    break;
                }

                case 5: {
                    t.displayAllKillerWinstreaks();
                    break;
                }

                case 6: {
                    while(true) {
                        std::print("[CONSOLE] Enter number of wins to set {}'s winstreak to: ", killer);
                        std::cin >> choice;
                        if(!inputHandling(choice)) {
                            continue;
                        }

                        break;
                    }

                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    t.specifyKillerWins(choice);
                    break;
                }
            }

        }
    }


}