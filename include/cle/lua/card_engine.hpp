#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <boost/leaf/error.hpp>
#include <boost/leaf/result.hpp>
#include <cle/core/card.hpp>
#include <cle/lua/bindings.hpp>
#include <sol/sol.hpp>

namespace cle::lua {

enum class CardLoadError { FileNotFound, ScriptError, InvalidReturnType, NullCard };

class CardEngine {
public:
    CardEngine() : lua_{std::make_unique<sol::state>()} {
        lua_->open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::math);
        register_all(*lua_);
    }

    [[nodiscard]] auto load_card_from_file(const std::filesystem::path& path)
        -> boost::leaf::result<std::shared_ptr<core::Card>> {
        if (!std::filesystem::exists(path)) {
            return boost::leaf::new_error(CardLoadError::FileNotFound, std::string{path.string()});
        }
        return execute_script([&] { return lua_->script_file(path.string()); });
    }

    [[nodiscard]] auto load_card_from_string(const std::string& lua_source)
        -> boost::leaf::result<std::shared_ptr<core::Card>> {
        return execute_script([&] { return lua_->script(lua_source); });
    }

    [[nodiscard]] auto lua_state() -> sol::state& { return *lua_; }

private:
    std::unique_ptr<sol::state> lua_;

    template <typename ScriptFn>
    auto execute_script(ScriptFn&& run_script) -> boost::leaf::result<std::shared_ptr<core::Card>> {
        try {
            auto result = run_script();

            if (!result.valid()) {
                sol::error err = result;
                return boost::leaf::new_error(CardLoadError::ScriptError, std::string{err.what()});
            }

            if (result.get_type() != sol::type::userdata) {
                return boost::leaf::new_error(CardLoadError::InvalidReturnType);
            }

            auto card = result.template get<std::shared_ptr<core::Card>>();
            if (!card) {
                return boost::leaf::new_error(CardLoadError::NullCard);
            }

            return card;

        } catch (const sol::error& e) {
            return boost::leaf::new_error(CardLoadError::ScriptError, std::string{e.what()});
        }
    }
};

}  // namespace cle::lua
