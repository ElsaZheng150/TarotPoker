cmake_minimum_required(VERSION 3.10)
project(TarotPoker)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(TarotPoker
    src/main.cpp
    src/Card.cpp
    src/Deck.cpp
    src/Player.cpp
    src/Input.cpp
    src/Computer.cpp
)
