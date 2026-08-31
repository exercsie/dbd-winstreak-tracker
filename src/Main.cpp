#include "Tracker.hpp"

#include <unordered_map>
#include <string>
#include <cstdint>
#include <print>
#include <iostream>
#include <limits>

int main() {
    std::string killer;
    std::uint16_t choice;
    Tracker t;
    
    std::println("Welcome to DBD winstreak tracker.");
    while(true) {
        bool isChoiceNewKiller = false;
        std::print("Enter your killer: ");
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
            std::println("{} does not exist", killer);
            continue;
        }

        while(true) {
            if(isChoiceNewKiller) {
                break;
            }

            std::println("Choose an option: ");
            std::println("1 - Start counting winstreak on {}", killer);
            std::println("2 - View winstreak on {}", killer);
            std::println("3 - Reset {}'s winstreak", killer);
            std::println("4 - Choose a new killer");
            std::println("5 - View all Killer's winstreaks");
            std::cin >> choice;
            if(std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::println(std::cerr, "[ERROR] Please enter a number!");
                continue;
            }

            if(choice < 1 || choice > 5) {
                std::cin.clear();
                std::println(std::cerr, "[ERROR] Please enter a number between 1 and 5");
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
            }

        }
    }


}