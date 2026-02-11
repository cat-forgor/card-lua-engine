cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(CardLuaEngine
    VERSION 0.1.0
    DESCRIPTION "MTG card definition library using Lua"
    LANGUAGES CXX C
)

# opts
option(MTGCLE_BUILD_TESTS "Build tests" ${PROJECT_IS_TOP_LEVEL})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# deps
include(IncludeSol2)
include(IncludeLeaf)

# check if lua is installed for sol2
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA REQUIRED lua5.4)

# lib
add_library(mtgcle INTERFACE)
add_library(CardLuaEngine::mtgcle ALIAS mtgcle)

target_include_directories(mtgcle
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${LUA_INCLUDE_DIRS}
)

target_link_libraries(mtgcle
    INTERFACE
    sol2::sol2
    boost::leaf
    ${LUA_LIBRARIES}
)

# tests
if(MTGCLE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
