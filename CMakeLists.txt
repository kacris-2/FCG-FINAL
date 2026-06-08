cmake_minimum_required(VERSION 3.28)

# Fixes the Freetype policy warning safely
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum policy version for all subprojects")
cmake_policy(SET CMP0048 NEW)

project(fcg-final LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set(SFML_BUILD_AUDIO OFF CACHE INTERNAL "")
set(SFML_BUILD_NETWORK OFF CACHE INTERNAL "")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

# EXECUTABLE DEFINITION FOR Tappa 01 
if(APPLE)
    add_executable(Tappa01 MACOSX_BUNDLE Tappa01/main.cpp)
else()
    add_executable(Tappa01 Tappa01/main.cpp)
endif()


target_link_libraries(Tappa01 PRIVATE SFML::Graphics)


if(APPLE)
    target_link_libraries(Tappa01 PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
    set_target_properties(Tappa01 PROPERTIES MACOSX_BUNDLE TRUE)
    set(CMAKE_MACOSX_RPATH 1)
endif()


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Tappa01)