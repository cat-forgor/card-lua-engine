#pragma once

#include <cstdint>

enum class CardType {
    Creature,
    Instant,
    Sorcery,
    Enchantment,
    Artifact,
    Planeswalker,
    Land
};

// Bitwise enum, thanks matt
enum class Color : uint8_t {
    White = 1 << 0,
    Blue = 1 << 1,
    Black = 1 << 2,
    Red = 1 << 3,
    Green = 1 << 4,
    Colorless = 0
};
