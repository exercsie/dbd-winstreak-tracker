#pragma once

#include <expected>
#include <string>

class GUI {
public:
    // Constructor
    GUI() = default;

    // Destructor
    ~GUI() = default;

    // Input Handling
    [[nodiscard]] std::expected<void, std::string> inputHandling(const int firstParam, const int secondParam, const std::string& errorMsg);
};