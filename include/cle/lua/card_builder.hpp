#pragma once

#include <string>
#include <unordered_map>

#include <boost/leaf/error.hpp>
#include <boost/leaf/result.hpp>
#include <cle/core/card.hpp>
#include <cle/core/enums.hpp>
#include <cle/mechanics/mana_cost.hpp>
#include <sol/sol.hpp>

namespace cle::lua {

class CardBuilder {
public:
    explicit CardBuilder(std::string name, cle::core::CardType type)
        : card_{std::make_shared<cle::core::Card>(std::move(name), type)} {}

    auto mana_cost(const std::string& mana_str) -> boost::leaf::result<CardBuilder&> {
        BOOST_LEAF_AUTO(cost, cle::mechanics::ManaCost::from_string(mana_str));
        card_->set_mana_cost(cost);
        return *this;
    }

    auto colors(std::vector<std::string> color_strings) -> boost::leaf::result<CardBuilder&> {
        cle::core::Color result = cle::core::Color::Colorless;
        for (const auto& c : color_strings) {
            BOOST_LEAF_AUTO(color, cle::core::color_from_string(c));  // NOLINT
            result = result | color;
        }
        card_->set_colors(result);
        return *this;
    }

    auto oracle_text(std::string text) -> CardBuilder& {
        card_->set_oracle_text(std::move(text));
        return *this;
    }

    auto flavor_text(std::string text) -> CardBuilder& {
        card_->set_flavor_text(std::move(text));
        return *this;
    }

    auto subtypes(std::vector<std::string> types) -> CardBuilder& {
        for (auto& t : types) {
            card_->add_subtype(std::move(t));
        }
        return *this;
    }

    auto keywords(std::vector<std::string> kws) -> CardBuilder& {
        for (auto& keyword : kws) {
            card_->add_keyword(std::move(keyword));
        }
        return *this;
    }

    auto power_toughness(int power, int toughness) -> CardBuilder& {
        card_->set_creature_stats(cle::core::CreatureStats{power, toughness});
        return *this;
    }

#define X(name, enum_val)                                            \
    auto name(sol::function fn) -> CardBuilder& {                    \
        triggers_[cle::core::TriggerType::enum_val] = std::move(fn); \
        return *this;                                                \
    }
    CARD_TRIGGER_LIST(X)
#undef X

#define X(name, str)              \
    auto name() -> CardBuilder& { \
        card_->add_keyword(str);  \
        return *this;             \
    }
    CARD_KEYWORD_LIST(X)
#undef X

    auto build() -> std::shared_ptr<cle::core::Card> {
        card_->set_triggers(std::move(triggers_));
        return card_;
    }

    [[nodiscard]] auto get_triggers() const
        -> const std::unordered_map<cle::core::TriggerType, sol::function>& {
        return triggers_;
    }

private:
    std::shared_ptr<cle::core::Card> card_;
    std::unordered_map<cle::core::TriggerType, sol::function> triggers_;
};

}  // namespace cle::lua
