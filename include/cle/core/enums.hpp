#pragma once

#include <cstdint>
#include <string>

#include <boost/leaf/error.hpp>
#include <boost/leaf/result.hpp>

namespace cle::core {

enum class CardType { Creature, Instant, Sorcery, Enchantment, Artifact, Planeswalker, Land };

// Bitwise enum, thanks matt
enum class Color : uint8_t {
    White = 1 << 0,
    Blue = 1 << 1,
    Black = 1 << 2,
    Red = 1 << 3,
    Green = 1 << 4,
    Colorless = 0
};

inline auto operator|(Color a, Color b) -> Color {
    return static_cast<Color>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline auto operator&(Color a, Color b) -> Color {
    return static_cast<Color>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline auto color_from_string(const std::string& s) -> boost::leaf::result<Color> {
    if (s == "W" || s == "White")
        return Color::White;
    if (s == "U" || s == "Blue")
        return Color::Blue;
    if (s == "B" || s == "Black")
        return Color::Black;
    if (s == "R" || s == "Red")
        return Color::Red;
    if (s == "G" || s == "Green")
        return Color::Green;

    return boost::leaf::new_error(std::string{"Invalid color"});
}

#define CARD_TRIGGER_LIST(X)                               \
    X(on_enter_battlefield, OnEnterBattlefield)            \
    X(on_leave_battlefield, OnLeaveBattlefield)            \
    X(on_death, OnDeath)                                   \
    X(on_cast, OnCast)                                     \
    X(on_draw, OnDraw)                                     \
    X(on_tap, OnTap)                                       \
    X(on_untap, OnUntap)                                   \
    X(on_damage_dealt, OnDamageDealt)                      \
    X(on_damage_taken, OnDamageTaken)                      \
    X(on_beginning_of_upkeep, BeginningOfUpkeep)           \
    X(on_end_of_turn, EndOfTurn)                           \
    X(on_attack, OnAttack)                                 \
    X(on_block, OnBlock)                                   \
    X(on_discard, OnDiscard)                               \
    X(on_sacrifice, OnSacrifice)                           \
    X(on_exile, OnExile)                                   \
    X(on_return_to_hand, OnReturnToHand)                   \
    X(on_milled, OnMilledFromLibrary)                      \
    X(on_targeted, OnTargeted)                             \
    X(on_countered, OnCountered)                           \
    X(on_beginning_of_combat, BeginningOfCombat)           \
    X(on_end_of_combat, EndOfCombat)                       \
    X(on_blocked_by, OnBlockedBy)                          \
    X(on_blocks_creature, OnBlocksCreature)                \
    X(on_deals_combat_damage, OnDealsCombatDamage)         \
    X(on_deals_player_damage, OnDealsPlayerDamage)         \
    X(on_life_gain, OnLifeGain)                            \
    X(on_life_loss, OnLifeLoss)                            \
    X(on_mana_added, OnManaAdded)                          \
    X(on_another_creature_enters, OnAnotherCreatureEnters) \
    X(on_another_creature_dies, OnAnotherCreatureDies)     \
    X(on_landfall, OnLandfall)                             \
    X(on_artifact_enters, OnArtifactEnters)                \
    X(on_enchantment_enters, OnEnchantmentEnters)          \
    X(on_spell_cast, OnSpellCast)                          \
    X(on_instant_or_sorcery_cast, OnInstantOrSorceryCast)  \
    X(on_beginning_of_end_step, BeginningOfEndStep)        \
    X(on_beginning_of_draw_step, BeginningOfDrawStep)      \
    X(on_beginning_of_main_phase, BeginningOfMainPhase)

#define CARD_KEYWORD_LIST(X)            \
    X(flying, "Flying")                 \
    X(trample, "Trample")               \
    X(haste, "Haste")                   \
    X(first_strike, "First Strike")     \
    X(double_strike, "Double Strike")   \
    X(deathtouch, "Deathtouch")         \
    X(lifelink, "Lifelink")             \
    X(vigilance, "Vigilance")           \
    X(reach, "Reach")                   \
    X(hexproof, "Hexproof")             \
    X(indestructible, "Indestructible") \
    X(menace, "Menace")                 \
    X(defender, "Defender")             \
    X(flash, "Flash")

enum class TriggerType {
#define X(method, enum_val) enum_val,
    CARD_TRIGGER_LIST(X)
#undef X
};

};  // namespace cle::core
