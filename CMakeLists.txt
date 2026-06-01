cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFMl
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(sfml-app
        src/main.cpp
        src/funckje.cpp
        src/Wierzcholek.h
        src/MethodMoveFunctions.cpp
        src/TypyStrategii.cpp

)

# Mówi CMake, żeby dołączył SFML do programu "sfml-app"
target_link_libraries(sfml-app PRIVATE sfml-graphics)
target_compile_features(sfml-app PUBLIC cxx_std_17) # Zachowaj to, jeśli masz
