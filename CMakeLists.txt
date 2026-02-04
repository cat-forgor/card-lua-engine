cmake_minimum_required(VERSION 3.30)

# Enable C++23 + modules
# ref: https://www.kitware.com/import-std-in-cmake-3-30/
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(CardLuaEngine
    VERSION 0.1.0
    DESCRIPTION "MTG card definition library using Lua"
    LANGUAGES CXX C
)

# opts
option(MTGCLE_BUILD_TESTS "Build tests" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# deps
include(IncludeSol2)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA REQUIRED lua5.4)

# lib
add_library(mtgcle SHARED
    src/main.cpp
)

add_library(CardLuaEngine::mtgcle ALIAS mtgcle)

target_include_directories(mtgcle
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${LUA_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(mtgcle
    PUBLIC
    sol2::sol2
    PRIVATE
    ${LUA_LIBRARIES}
)

set_target_properties(mtgcle PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME "mtgcle"
    DEBUG_POSTFIX "d"
    # performance stuff apparently
    VISIBILITY_INLINES_HIDDEN ON
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET hidden
)

# Tests
if(MTGCLE_BUILD_TESTS)
    enable_testing()
    # add_subdirectory(tests)  # Still need to add tests
endif()

# TODO: install rules (im lazy)
