cmake_minimum_required(VERSION 3.31)
project(game_of_life)
set(CMAKE_CXX_STANDARD 20)

#find_package(raylib CONFIG REQUIRED)
#add_executable(game_of_life main.cpp)
#target_link_libraries(game_of_life PRIVATE raylib)

include_directories("./external/raylib-5.5_macos/include")

link_directories("./external/raylib-5.5_macos/lib")

add_executable(game_of_life src/main.cpp src/grid.cpp src/simulation.cpp)
target_link_libraries(game_of_life raylib)

