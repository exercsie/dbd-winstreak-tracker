#pragma once

#include "Tracker.hpp"
#include "../../Assets/Fonts/NotoSans/NotoSans.hpp"
#include "../../Assets/Images/Killers/THE-BLIGHT.hpp"
#include "../../Assets/Images/Killers/THE-CANNIBAL.hpp"
#include "../../Assets/Images/Killers/THE-DEATHSLINGER.hpp"
#include "../../Assets/Images/Killers/THE-DEMOGORGON.hpp"
#include "../../Assets/Images/Killers/THE-GHOUL.hpp"
#include "../../Assets/Images/Killers/THE-HILLBILLY.hpp"
#include "../../Assets/Images/Killers/THE-NURSE.hpp"
#include "../../Assets/Images/Killers/THE-ANIMATRONIC.hpp"
#include "../../Assets/Images/Killers/THE-CENOBITE.hpp"
#include "../../Assets/Images/Killers/THE-CLOWN.hpp"
#include "../../Assets/Images/Killers/THE-DARK-LORD.hpp"
#include "../../Assets/Images/Killers/THE-DOCTOR.hpp"
#include "../../Assets/Images/Killers/THE-DREDGE.hpp"
#include "../../Assets/Images/Killers/THE-EXECUTIONER.hpp"
#include "../../Assets/Images/Killers/THE-FIRST.hpp"
#include "../../Assets/Images/Killers/THE-GHOST-FACE.hpp"
#include "../../Assets/Images/Killers/THE-GOOD-GUY.hpp"
#include "../../Assets/Images/Killers/THE-HAG.hpp"
#include "../../Assets/Images/Killers/THE-HOUNDMASTER.hpp"
#include "../../Assets/Images/Killers/THE-HUNTRESS.hpp"
#include "../../Assets/Images/Killers/THE-JUDGEMENT.hpp"
#include "../../Assets/Images/Killers/THE-KNIGHT.hpp"
#include "../../Assets/Images/Killers/THE-KRASUE.hpp"
#include "../../Assets/Images/Killers/THE-LEGION.hpp"
#include "../../Assets/Images/Killers/THE-LICH.hpp"
#include "../../Assets/Images/Killers/THE-MASTERMIND.hpp"
#include "../../Assets/Images/Killers/THE-NEMESIS.hpp"
#include "../../Assets/Images/Killers/THE-NIGHTMARE.hpp"
#include "../../Assets/Images/Killers/THE-ONI.hpp"
#include "../../Assets/Images/Killers/THE-ONRYO.hpp"
#include "../../Assets/Images/Killers/THE-PIG.hpp"
#include "../../Assets/Images/Killers/THE-PLAGUE.hpp"
#include "../../Assets/Images/Killers/THE-SHAPE.hpp"
#include "../../Assets/Images/Killers/THE-SINGULARITY.hpp"
#include "../../Assets/Images/Killers/THE-SKULL-MERCHANT.hpp"
#include "../../Assets/Images/Killers/THE-SLASHER.hpp"
#include "../../Assets/Images/Killers/THE-SPIRIT.hpp"
#include "../../Assets/Images/Killers/THE-TERRIFIER.hpp"
#include "../../Assets/Images/Killers/THE-TWINS.hpp"
#include "../../Assets/Images/Killers/THE-UNKNOWN.hpp"
#include "../../Assets/Images/Killers/THE-WRAITH.hpp"
#include "../../Assets/Images/Killers/THE-XENOMORPH.hpp"
#include "../../Assets/Images/Killers/THE-ARTIST.hpp"
#include "../../Assets/Images/Killers/THE-TRICKSTER.hpp"
#include "../../Assets/Images/Killers/THE-TRAPPER.hpp"

#include <cstdint>          // std::uint8_t, std::uint32_t
#include <optional>         // std::optional
#include <unordered_map>    // std::unorderd_map

class GUI {
public:
    struct SourceImage {
        const std::uint8_t* bytes;
        int length;
    };

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
    void KillerView(ImVec2& displaySize, bool& killerSelected, const std::string& selectedKiller);

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
private:
    // Image memory map: Killer onto {rawBytes, sizeof(rawBytes)}
    inline static const std::unordered_map<std::string, SourceImage> killerImages {
        {"THE-BLIGHT", {K21_TheBlight_Portrait_png, K21_TheBlight_Portrait_png_len}},
        {"THE-CANNIBAL", {K09_TheCannibal_Portrait_png, K09_TheCannibal_Portrait_png_len}},
        {"THE-DEATHSLINGER", {K19_TheDeathslinger_Portrait_png, K19_TheDeathslinger_Portrait_png_len}},
        {"THE-DEMOGORGON", {K17_TheDemogorgon_Portrait_png, K17_TheDemogorgon_Portrait_png_len}},
        {"THE-GHOUL", {K39_TheGhoul_Portrait_png, K39_TheGhoul_Portrait_png_len}},
        {"THE-HILLBILLY", {K03_TheHillbilly_Portrait_png, K03_TheHillbilly_Portrait_png_len}},
        {"THE-NURSE", {K04_TheNurse_Portrait_png, K04_TheNurse_Portrait_png_len}},
        {"THE-CLOWN", {K12_TheClown_Portrait_png, K12_TheClown_Portrait_png_len}},
        {"THE-ARTIST", {K26_TheArtist_Portrait_png, K26_TheArtist_Portrait_png_len}},
        {"THE-GOOD-GUY", {K34_TheGoodGuy_Portrait_png, K34_TheGoodGuy_Portrait_png_len}},
        {"THE-GHOST-FACE", {K16_TheGhostFace_Portrait_png, K16_TheGhostFace_Portrait_png_len}},
        {"THE-PIG", {K11_ThePig_Portrait_png, K11_ThePig_Portrait_png_len}},
        {"THE-NEMESIS", {K24_TheNemesis_Portrait_png, K24_TheNemesis_Portrait_png_len}},
        {"THE-DOCTOR", {K07_TheDoctor_Portrait_png, K07_TheDoctor_Portrait_png_len}},
        {"THE-FIRST", {K42_TheFirst_Portrait_png, K42_TheFirst_Portrait_png_len}},
        {"THE-LEGION", {K14_TheLegion_Portrait_png, K14_TheLegion_Portrait_png_len}},
        {"THE-TERRIFIER", {K45_TheTerrifier_Portrait_png, K45_TheTerrifier_Portrait_png_len}},
        {"THE-TWINS", {K22_TheTwins_Portrait_png, K22_TheTwins_Portrait_png_len}},
        {"THE-SKULL-MERCHANT", {K31_TheSkullMerchant_Portrait_png, K31_TheSkullMerchant_Portrait_png_len}},
        {"THE-JUDGEMENT", {K44_TheJudgment_Portrait_png, K44_TheJudgment_Portrait_png_len}},
        {"THE-KNIGHT", {K30_TheKnight_Portrait_png, K30_TheKnight_Portrait_png_len}},
        {"THE-TRICKSTER", {K23_TheTrickster_Portrait_png, K23_TheTrickster_Portrait_png_len}},
        {"THE-HUNTRESS", {K08_TheHuntress_Portrait_png, K08_TheHuntress_Portrait_png_len}},   
        {"THE-LICH", {K36_TheLich_Portrait_png, K36_TheLich_Portrait_png_len}},   
        {"THE-HAG", {K06_TheHag_Portrait_png,K06_TheHag_Portrait_png_len}},
        {"THE-MASTERMIND", {K29_TheMastermind_Portrait_png, K29_TheMastermind_Portrait_png_len}},
        {"THE-UNKNOWN", {K35_TheUnknown_Portrait_png, K35_TheUnknown_Portrait_png_len}},
        {"THE-NIGHTMARE", {K10_TheNightmare_Portrait_png, K10_TheNightmare_Portrait_png_len}},
        {"THE-ONRYO", {K27_TheOnryo_Portrait_png, K27_TheOnryo_Portrait_png_len}},
        {"THE-ONI", {K18_TheOni_Portrait_png, K18_TheOni_Portrait_png_len}},
        {"THE-WRAITH", {K02_TheWraith_Portrait_png, K02_TheWraith_Portrait_png_len}},
        {"THE-KRASUE", {K41_TheKrasue_Portrait_png, K41_TheKrasue_Portrait_png_len}},
        {"THE-TRAPPER", {K01_TheTrapper_Portrait_png, K01_TheTrapper_Portrait_png_len}},
        {"THE-XENOMORPH", {K33_TheXenomorph_Portrait_png, K33_TheXenomorph_Portrait_png_len}},
        {"THE-SLASHER", {K43_TheSlasher_Portrait_png, K43_TheSlasher_Portrait_png_len}},
        {"THE-CENOBITE", {K25_TheCenobite_Portrait_png, K25_TheCenobite_Portrait_png_len}},
        {"THE-DREDGE", {K28_TheDredge_Portrait_png, K28_TheDredge_Portrait_png_len}},
        {"THE-DARK-LORD", {K37_TheDarkLord_Portrait_png, K37_TheDarkLord_Portrait_png_len}},
        {"THE-ANIMATRONIC", {K40_TheAnimatronic_Portrait_png, K40_TheAnimatronic_Portrait_png_len}},
        {"THE-SPIRIT", {K13_TheSpirit_Portrait_png, K13_TheSpirit_Portrait_png_len}},
        {"THE-SHAPE", {K05_TheShape_Portrait_png, K05_TheShape_Portrait_png_len}},
        {"THE-PLAGUE", {K15_ThePlague_Portrait_png, K15_ThePlague_Portrait_png_len}},
        {"THE-EXECUTIONER", {K20_TheExecutioner_Portrait_png, K20_TheExecutioner_Portrait_png_len}},
        {"THE-SINGULARITY", {K32_TheSingularity_Portrait_png, K32_TheSingularity_Portrait_png_len}}
    };
};