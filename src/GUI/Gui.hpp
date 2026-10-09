#pragma once

#include "Tracker.hpp"
#include "../../Assets/Fonts/NotoSans/NotoSans.hpp"

#include <cstdint>          // std::uint8_t, std::uint32_t

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

    // Query
    void QueryOption(const Tracker& t, ImVec2& displaySize, const std::string& selectedKiller);
    void displayAllKillerStats(const Tracker& t) const noexcept;
    void displayKillerWinstreaksInReferenceToN(const Tracker& t, const std::uint32_t n) const noexcept;
    void displayKillerPersonalBestsInReferenceToN(const Tracker& t, const std::uint32_t n) const noexcept;

    // Killer View
    void KillerOption(ImVec2& displaySize);

    // Font wrapper
    [[nodiscard]] ImFont* staticFontLoader(std::uint8_t* fontData, const std::uint32_t fontLength, float size = 20.0f);
};