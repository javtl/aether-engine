cmake_minimum_required(VERSION 3.14)

# Definición del proyecto
project(AetherEngine 
    VERSION 1.0.0
    LANGUAGES CXX
)

# Forzar el estándar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Exportar información de compilación (útil para IDEs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Opciones de compilación (Warnings) - Fuerzan código limpio
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
endif()

# Opción para activar/desactivar tests
option(BUILD_TESTING "Build the testing suite" ON)

# ============================================================================
# BIBLIOTECA CORE (GameFSM + CryptoEngine)
# ============================================================================
add_library(aether_core STATIC
    src/Fsm.cpp
    src/CryptoEngine.cpp
)

# Definir dónde están los archivos de cabecera (.hpp)
target_include_directories(aether_core 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ============================================================================
# EJECUTABLE PRINCIPAL
# ============================================================================
add_executable(aether_main
    src/main.cpp
)

# Enlazar la biblioteca core al ejecutable
target_link_libraries(aether_main 
    PRIVATE 
        aether_core
)

# Darle nombre bonito al ejecutable
set_target_properties(aether_main PROPERTIES
    OUTPUT_NAME "aether_engine"
)

# ============================================================================
# TESTS CON GOOGLETEST
# ============================================================================
if(BUILD_TESTING)
    include(CTest)
    include(FetchContent)

    # Descargar GoogleTest automáticamente
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Crear ejecutable de tests
    add_executable(aether_tests
        tests/test_fsm.cpp
        tests/test_crypto.cpp
    )

    # Enlazar tests con la biblioteca core y GoogleTest
    target_link_libraries(aether_tests
        PRIVATE
            aether_core
            GTest::gtest_main
    )

    # Registrar tests en CTest
    include(GoogleTest)
    gtest_discover_tests(aether_tests)
endif()

# Mensaje informativo al terminar de configurar
message(STATUS "=================================================")
message(STATUS "Proyecto: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "=================================================")