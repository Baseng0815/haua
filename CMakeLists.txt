cmake_minimum_required(VERSION 3.16)

project(haua
    VERSION 0.1.0
    DESCRIPTION "Home Automation Unified Architecture"
    LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(UA_ENABLE_AMALGAMATION OFF CACHE BOOL "" FORCE)
add_subdirectory(external/open62541)

add_compile_options(-Wno-c++-compat)

add_subdirectory(node/generic)
add_subdirectory(led)
add_subdirectory(node/x86)
