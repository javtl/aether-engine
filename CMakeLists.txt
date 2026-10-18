cmake_minimum_required(VERSION 3.20)

project(AetherEngine 
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ============ BIBLIOTECA CORE ============
add_library(aether_core STATIC
    src/Fsm.cpp
)

target_include_directories(aether_core 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============ EJECUTABLE PRINCIPAL ============
add_executable(aether_engine
    src/main.cpp
)

target_link_libraries(aether_engine 
    PRIVATE 
        aether_core
)

# ============ TESTS ============
include(CTest)
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(aether_tests
    tests/FsmTests.cpp
)

target_link_libraries(aether_tests
    PRIVATE
        aether_core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(aether_tests)

message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION} | C++${CMAKE_CXX_STANDARD}")