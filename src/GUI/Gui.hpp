#pragma once

#include "Tracker.hpp"

class GUI {
private:
    Tracker t;
public:
    // UI Menu button enums
    enum class UI { 
        counter,
        resetStats,
        setStats,
        query
    };

    // Constructor
    GUI() = default;

    // Destructor
    ~GUI() = default;

    // Button UI
    bool buttonColour(const char* name, ImVec4 v, ImVec2 size = ImVec2(100, 0));

    // Query logic
    void displayAllKillerStats(const Tracker& t) const noexcept;
    void displayKillerWinstreaksInReferenceToN(const Tracker& t, const int n) const noexcept;
    void displayKillerPersonalBestsInReferenceToN(const Tracker& t, const int n) const noexcept;
};