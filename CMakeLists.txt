cmake_minimum_required(VERSION 3.20)
project(BreakoutFinal)

set(CMAKE_CXX_STANDARD 17)

set(SFML_INCLUDE_DIR "/usr/local/include")
set(SFML_LIBRARY_DIR "/usr/local/lib")
set(CMAKE_OSX_ARCHITECTURES x86_64)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

include_directories(${APP_DOMAIN})
include_directories(${APP_CALC})

add_executable(BreakoutFinal breakout.cpp breakout_defs.h)
target_link_libraries(BreakoutFinal sfml-graphics sfml-window sfml-system)