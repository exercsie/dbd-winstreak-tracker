#pragma once

#include <expected>
#include <string>

class GUI {
public:
    // UI Menu button enums
    enum class UI { 
        counter,
        viewStats,
        resetStats,
        setStats,
        query
    };

    // Constructor
    GUI() = default;

    // Destructor
    ~GUI() = default;

    // Input Handling
    [[nodiscard]] std::expected<void, std::string> inputHandling(const int firstParam, const int secondParam, const std::string& errorMsg);
};