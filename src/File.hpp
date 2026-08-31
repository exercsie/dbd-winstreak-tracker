#pragma once

#include <fstream>
#include <iostream>
#include <unordered_map>

class File {
public:
    void killerNameChecker(const std::unordered_map<std::string, std::uint16_t>&);
    void readFromFile();
    void addToFile();
private:

};