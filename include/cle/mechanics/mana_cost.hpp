#pragma once

#include <cctype>
#include <charconv>
#include <string_view>
#include <vector>

#include <boost/leaf/error.hpp>
#include <boost/leaf/result.hpp>

namespace cle::mechanics {

enum class ManaColor : uint8_t { White, Blue, Black, Red, Green, Colorless };

enum class ManaParseError { InvalidFormat, UnknownToken };

struct HybridCost {
    ManaColor primary;
    ManaColor secondary;
};

inline auto char_to_color(char c) -> boost::leaf::result<ManaColor> {
    switch (c) {
        case 'W':
            return ManaColor::White;
        case 'U':
            return ManaColor::Blue;
        case 'B':
            return ManaColor::Black;
        case 'R':
            return ManaColor::Red;
        case 'G':
            return ManaColor::Green;
        case 'C':
            return ManaColor::Colorless;
        default:
            return boost::leaf::new_error(ManaParseError::UnknownToken);
    }
}

struct ManaCost {
    int colorless{0}, white{0}, blue{0}, black{0}, red{0}, green{0}, x_count{0};
    std::vector<HybridCost> hybrid_costs;

    // parse mana strings to a ManaCost
    static auto from_string(std::string_view input) -> boost::leaf::result<ManaCost> {
        if (input.empty() || input.front() != '{' || input.back() != '}') {
            return boost::leaf::new_error(ManaParseError::InvalidFormat);
        }

        ManaCost cost;

        size_t start = 1;
        for (size_t i = 1; i < input.size(); ++i) {
            if (input[i] == '}') {
                std::string_view token = input.substr(start, i - start);
                BOOST_LEAF_CHECK(cost.apply_token(token));  // NOLINT
                start = i + 2;
            }
        }

        return cost;
    }

    // cmc
    [[nodiscard]] auto mana_value() const noexcept -> int {
        int total = colorless + white + blue + black + red + green;

        // Hybrid contribute based on their lower cost
        for (const auto &hybrid : hybrid_costs) {
            total += (hybrid.primary == ManaColor::Colorless) ? 2 : 1;
        }

        return total;
    }

private:
    auto apply_token(std::string_view token) -> boost::leaf::result<void> {
        if (token.empty()) {
            return boost::leaf::new_error(ManaParseError::InvalidFormat);
        }

        // x cost
        if (token == "X") {
            x_count++;
            return {};
        }

        // hybrids
        if (token.find('/') != std::string_view::npos) {
            BOOST_LEAF_CHECK(parse_hybrid(token));  // NOLINT
            return {};
        }

        // colorless
        if (std::isdigit(static_cast<unsigned char>(token[0]))) {
            BOOST_LEAF_CHECK(parse_colorless(token));  // NOLINT
            return {};
        }

        // plain symbols
        BOOST_LEAF_CHECK(parse_color(token));  // NOLINT
        return {};
    }

    auto parse_colorless(std::string_view token) -> boost::leaf::result<void> {
        int value = 0;
        auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), value);

        if (ec != std::errc{} || ptr != token.data() + token.size()) {
            return boost::leaf::new_error(ManaParseError::UnknownToken);
        }

        colorless += value;
        return {};
    }

    auto parse_color(std::string_view token) -> boost::leaf::result<void> {
        if (token.size() != 1) {
            return boost::leaf::new_error(ManaParseError::InvalidFormat);
        }
        BOOST_LEAF_AUTO(color_result, char_to_color(token[0]));  // NOLINT
        add_color(color_result);
        return {};
    }

    auto parse_hybrid(std::string_view token) -> boost::leaf::result<void> {
        size_t slash_pos = token.find('/');
        if (slash_pos == std::string_view::npos || slash_pos == 0 ||
            slash_pos == token.size() - 1) {
            return boost::leaf::new_error(ManaParseError::InvalidFormat);
        }

        std::string_view first_part = token.substr(0, slash_pos);
        std::string_view second_part = token.substr(slash_pos + 1);

        // Second part must be exactly one color character
        if (second_part.size() != 1) {
            return boost::leaf::new_error(ManaParseError::InvalidFormat);
        }
        BOOST_LEAF_AUTO(second_color, char_to_color(second_part[0]));  // NOLINT

        // First part: either a single digit for colorless or a single color char
        if (first_part.size() != 1) {
            return boost::leaf::new_error(ManaParseError::InvalidFormat);
        }
        BOOST_LEAF_AUTO(first_color,  // NOLINT
                        std::isdigit(static_cast<unsigned char>(first_part[0]))
                            ? boost::leaf::result<ManaColor>{ManaColor::Colorless}
                            : char_to_color(first_part[0]));

        hybrid_costs.push_back({first_color, second_color});
        return {};
    }

    void add_color(ManaColor color) noexcept {
        switch (color) {
            case ManaColor::White:
                white++;
                break;
            case ManaColor::Blue:
                blue++;
                break;
            case ManaColor::Black:
                black++;
                break;
            case ManaColor::Red:
                red++;
                break;
            case ManaColor::Green:
                green++;
                break;
            case ManaColor::Colorless:
                colorless++;
                break;
        }
    }
};

}  // namespace cle::mechanics