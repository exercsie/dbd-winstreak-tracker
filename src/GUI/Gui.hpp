#pragma once

#include "Tracker.hpp"
#include "../../Assets/Fonts/NotoSans/NotoSans.hpp"

#include <cstdint>          // std::uint8_t, std::uint32_t
#include <optional>         // std::optional

class GUI {
public:
    struct ImageDetails {
        float width{};
        float height{};
    };

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

    // Views
    void MenuView(Tracker& t, ImVec2& displaySize, bool& killerSelected, std::string& selectedKiller, std::string& error, std::string& success);
    void KillerView(ImVec2& displaySize);

    // UI
    bool buttonColour(const char* name, ImVec4 v, ImVec2 size = ImVec2(100, 0));
    void CounterOption(Tracker& t, std::string& error, const std::string& selectedKiller);
    void ResetStatsOption(Tracker& t, std::string& error, std::string& success, const std::string& selectedKiller);
    void SetStatsOption(Tracker& t, std::string& error, std::string& success);
    void QueryOption(const Tracker& t, ImVec2& displaySize, const std::string& selectedKiller);

    // Query display logic
    void displayAllKillerStats(const Tracker& t) const noexcept;
    void displayKillerWinstreaksInReferenceToN(const Tracker& t, const std::uint32_t n) const noexcept;
    void displayKillerPersonalBestsInReferenceToN(const Tracker& t, const std::uint32_t n) const noexcept;

    // Image wrapper
    [[nodiscard]] static std::optional<std::uint32_t> imageLoader(const std::uint8_t* bytes, int length, ImageDetails& details);

    // Font wrapper
    [[nodiscard]] ImFont* staticFontLoader(std::uint8_t* fontData, const std::uint32_t fontLength, float size = 20.0f);
};