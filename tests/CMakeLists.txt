include(IncludeCatch2)
include(Catch)

add_executable(mtgcle_tests
    unit/test_card.cpp
    unit/test_mana_cost.cpp
)

target_link_libraries(mtgcle_tests
    PRIVATE
    Catch2::Catch2WithMain
    CardLuaEngine::mtgcle
)

catch_discover_tests(mtgcle_tests)
