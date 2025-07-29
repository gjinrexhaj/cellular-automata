cmake_minimum_required(VERSION 3.28.3)
project(cellular-automata)
set(CMAKE_CXX_STANDARD 20)


if(APPLE)
    message("Building on OSX!")
    include_directories("./external/raylib-5.5_macos/include")
    link_directories("./external/raylib-5.5_macos/lib")
endif ()

if(WIN32)
    message("Building on Windows!")
    include_directories("./external/raylib-5.5_win64_msvc16/include")
    link_directories("./external/raylib-5.5_win64_msvc16/lib")
endif ()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Building on Linux!")
    include_directories("./external/raylib-5.5_linux_amd64/include")
    link_directories("./external/raylib-5.5_linux_amd64/lib")
endif ()


add_executable(cellular-automata src/main.cpp src/grid.cpp src/simulation.cpp)


# Explicity link win multimedia if building for win32
if (WIN32)
    target_link_libraries(cellular-automata raylib Winmm)
else ()
    target_link_libraries(cellular-automata raylib)
endif ()

