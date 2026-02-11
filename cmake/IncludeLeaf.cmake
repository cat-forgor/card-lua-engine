include(FetchContent)

FetchContent_Declare(
    leaf
    GIT_REPOSITORY https://github.com/boostorg/leaf.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(leaf)

target_include_directories(boost_leaf SYSTEM INTERFACE
    ${leaf_SOURCE_DIR}/include
)

if(NOT TARGET boost::leaf)
    add_library(boost::leaf ALIAS boost_leaf)
endif()
