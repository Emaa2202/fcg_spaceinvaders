cmake_minimum_required(VERSION 3.28)

#Nome progetto
project(SpaceInvaders LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

#Eseguibile principale
add_executable(SpaceInvaders
    src/main.cpp
    src/stateUpdate.cpp
)
target_compile_features(SpaceInvaders PRIVATE cxx_std_17)

#Librerie opzionali con target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio SFML::Network)
target_link_libraries(SpaceInvaders PRIVATE SFML::Graphics)
target_link_libraries(SpaceInvaders PRIVATE SFML::Audio)
#Cartella media dalla root alla build
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SpaceInvaders)

