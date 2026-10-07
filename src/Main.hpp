#pragma once

#include <cstdint>          // std::uint16_t
#include <expected>         // std::expected
#include <string>           // std::string

class Menu {
public:
    // Constructor
    Menu() = default;
    
    // Destructor
    ~Menu() = default;

    // Input handling
    [[nodiscard]] std::expected<void, std::string> inputHandling(const int choice, const std::uint16_t lowerBound = 0, const std::uint16_t upperBound = 65535);
    void killerAliases(std::string& k) noexcept;
};