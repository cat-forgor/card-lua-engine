#pragma once

#include <string>
#include <vector>
#include <atomic>

#include "enums.hpp"
#include "mana_cost.hpp"

namespace CLE {
    class Card {
        public:
            Card(std::string name, CardType type)
                : name_{std::move(name)}
                , type_{type}
                , instance_id_{next_instance_id_++} {}

            // create a new card and assign a cards data to it and a new id
            Card(const Card& other)
                : name_{other.name_}
                , type_{other.type_}
                , mana_cost_{other.mana_cost_}
                , colors_{other.colors_}
                , oracle_text_{other.oracle_text_}
                , flavor_text_{other.flavor_text_}
                , subtypes_{other.subtypes_}
                , keywords_{other.keywords_}
                , instance_id_{next_instance_id_++} {}  // Gen new id

            // assign a cards data to another existing card and give it a new id
            Card& operator=(const Card& other) {
                if (this != &other) {   // dont self assign
                    name_ = other.name_;
                    type_ = other.type_;
                    mana_cost_ = other.mana_cost_;
                    colors_ = other.colors_;
                    oracle_text_ = other.oracle_text_;
                    flavor_text_ = other.flavor_text_;
                    subtypes_ = other.subtypes_;
                    keywords_ = other.keywords_;
                    instance_id_ = next_instance_id_++;  // gen new id
                }
                return *this;
            }

            Card(Card&&) noexcept = default;
            Card& operator=(Card&&) noexcept = default;

            // getters
            [[nodiscard]] auto instance_id() const -> uint64_t { return instance_id_; }
            [[nodiscard]] auto name() const -> const std::string& { return name_; }
            [[nodiscard]] auto type() const -> CardType { return type_; }
            [[nodiscard]] auto mana_cost() const -> const ManaCost& { return mana_cost_; }
            [[nodiscard]] auto colors() const -> Color { return colors_; }
            [[nodiscard]] auto oracle_text() const -> const std::string& { return oracle_text_; }
            [[nodiscard]] auto flavor_text() const -> const std::string& { return flavor_text_; }
            [[nodiscard]] auto subtypes() const -> const std::vector<std::string>& { return subtypes_; }
            [[nodiscard]] auto keywords() const -> const std::vector<std::string>& { return keywords_; }

            // setters
            void set_mana_cost(ManaCost cost) { mana_cost_ = cost; }
            void set_colors(Color colors) { colors_ = colors; }
            void set_oracle_text(std::string text) { oracle_text_ = std::move(text); }
            void set_flavor_text(std::string text) { flavor_text_ = std::move(text); }
            void add_subtype(std::string subtype) { subtypes_.push_back(std::move(subtype)); }
            void add_keyword(std::string keyword) { keywords_.push_back(std::move(keyword)); }

        private:
            /* 
                unique instance id for each card, not scalable 
                to multiple servers but its fine for this use-case 
            */
            static inline std::atomic<uint64_t> next_instance_id_{1};

            std::string              name_;
            CardType                 type_;
            ManaCost                 mana_cost_;
            Color                    colors_;
            std::string              oracle_text_;
            std::string              flavor_text_;
            std::vector<std::string> subtypes_;
            std::vector<std::string> keywords_;
            uint64_t                 instance_id_; // to prevent same cards but different instance being registered as the same

    };
};
