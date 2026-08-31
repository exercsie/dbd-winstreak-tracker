#include "File.hpp"

#include <fstream>
#include <unordered_map>
#include <print>
#include <iostream>

void File::killerNameChecker(const std::unordered_map<std::string, std::uint16_t>&) {
    std::ofstream trackerFile("../Files/killer_win_info.txt", std::ios::app);

    if(!trackerFile) {
        (std::cerr, "Failed to open killer_win_info.txt");
        throw std::runtime_error("Failed to open killer_win_info.txt");
    }

    trackerFile.close();

}