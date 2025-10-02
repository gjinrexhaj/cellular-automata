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

if(EMSCRIPTEN)
    message("Building for WASM")
    include_directories("./external/raylib-5.5_webassembly/include")
    include_directories("./external/raylib-5.5_webassembly/lib")
endif()

# Group all sources and headers
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h src/*.hpp)
file(GLOB_RECURSE STYLES styles/*.h)

set(ICON_FILE "AppIcon.icns")
set(ICON_PATH "./Resources/${ICON_FILE}")
set(RC_PATH "./Resources/app.rc")




add_executable(cellular-automata ${SOURCES} ${HEADERS} ${STYLES} ${ICON_PATH} ${RC_PATH})



# Build macos app as bundle
if (APPLE)
    message("Building macOS app bundle")
    set_target_properties(cellular-automata PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_ICON_FILE ${ICON_FILE}
    )
    set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()


# Explicitly link win multimedia if building for win32
if (WIN32)
    target_link_libraries(cellular-automata raylib Winmm)
else ()
    target_link_libraries(cellular-automata raylib)
endif ()