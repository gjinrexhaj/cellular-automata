cmake_minimum_required(VERSION 3.31)
project(game_of_life)
set(CMAKE_CXX_STANDARD 20)


if(APPLE)
    message("Building on OSX!")
    include_directories("./external/raylib-5.5_macos/include")
    link_directories("./external/raylib-5.5_macos/lib")
endif ()

if(WIN32)
    message("Building on Windows!")
    include_directories("./external/raylib-5.5_win64_mingw-w64/include")
    link_directories("./external/raylib-5.5_win64_mingw-w64/lib")
endif ()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Building on Linux!")
    include_directories("./external/raylib-5.5_linux_amd64/include")
    link_directories("./external/raylib-5.5_linux_amd64/lib")
endif ()


add_executable(game_of_life src/main.cpp src/grid.cpp src/simulation.cpp)
target_link_libraries(game_of_life raylib)

