#pragma once

#include <cle/core/card.hpp>
#include <cle/core/enums.hpp>
#include <cle/lua/card_builder.hpp>
#include <cle/mechanics/mana_cost.hpp>
#include <sol/sol.hpp>

namespace cle::lua {

inline void register_enums(sol::state& lua) {
    lua.new_enum<core::CardType>("CardType", {{"Creature", core::CardType::Creature},
                                              {"Instant", core::CardType::Instant},
                                              {"Sorcery", core::CardType::Sorcery},
                                              {"Enchantment", core::CardType::Enchantment},
                                              {"Artifact", core::CardType::Artifact},
                                              {"Planeswalker", core::CardType::Planeswalker},
                                              {"Land", core::CardType::Land}});

    lua.new_enum<core::Color>("Color", {{"White", core::Color::White},
                                        {"Blue", core::Color::Blue},
                                        {"Black", core::Color::Black},
                                        {"Red", core::Color::Red},
                                        {"Green", core::Color::Green},
                                        {"Colorless", core::Color::Colorless}});
}

inline void register_card_builder(sol::state& lua) {
    auto mana_cost_wrapper = [](CardBuilder& builder, const std::string& mana_str) -> CardBuilder& {
        auto result = builder.mana_cost(mana_str);
        if (!result) {
            throw std::runtime_error("Invalid mana cost format: " + mana_str);
        }
        return result.value();
    };

    auto colors_wrapper = [](CardBuilder& builder,
                             std::vector<std::string> color_strings) -> CardBuilder& {
        auto result = builder.colors(std::move(color_strings));
        if (!result) {
            throw std::runtime_error("Invalid color string");
        }
        return result.value();
    };

    // clang-format was making the macros TWEAK out
    // clang-format off
    lua.new_usertype<CardBuilder>("CardBuilder", sol::no_constructor,
        "mana_cost", mana_cost_wrapper,
        "colors", colors_wrapper,
        "oracle_text", &CardBuilder::oracle_text,
        "flavor_text", &CardBuilder::flavor_text,
        "subtypes", &CardBuilder::subtypes,
        "keywords", &CardBuilder::keywords,
        "power_toughness", &CardBuilder::power_toughness,
#define X(name, _) #name, &CardBuilder::name,
        CARD_TRIGGER_LIST(X)
        CARD_KEYWORD_LIST(X)
#undef X
        "build", &CardBuilder::build
    );
    // clang-format on
}

inline void register_card(sol::state& lua) {
    lua.new_usertype<core::Card>(
        "Card", sol::no_constructor, "name", &core::Card::name, "type", &core::Card::type,
        "mana_cost", &core::Card::mana_cost, "colors", &core::Card::colors, "oracle_text",
        &core::Card::oracle_text, "flavor_text", &core::Card::flavor_text, "subtypes",
        &core::Card::subtypes, "keywords", &core::Card::keywords, "creature_stats",
        &core::Card::creature_stats, "instance_id", &core::Card::instance_id);
}

inline void register_factory_functions(sol::state& lua) {
#define FACTORY(type) \
    lua[#type] = [](const std::string& name) { return CardBuilder(name, core::CardType::type); };
    FACTORY(Creature)
    FACTORY(Instant)
    FACTORY(Sorcery)
    FACTORY(Enchantment)
    FACTORY(Artifact)
    FACTORY(Planeswalker)
    FACTORY(Land)
#undef FACTORY
}

inline void register_all(sol::state& lua) {
    register_enums(lua);
    register_card(lua);
    register_card_builder(lua);
    register_factory_functions(lua);
}

}  // namespace cle::lua
