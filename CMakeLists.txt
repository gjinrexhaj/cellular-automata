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
    link_directories("./external/raylib-5.5_webassembly/lib")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
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
    #set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    message("Adding icon to bundle resource")
    set_source_files_properties(${ICON_PATH} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
    )

    message("Adding fonts to bundle resource")
    set_source_files_properties(${APP_FONTS} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
    )

    # Patch the dylibs into the bundle
    set_target_properties(${PROJECT_NAME} PROPERTIES
            BUILD_RPATH "@executable_path/../Frameworks"
            INSTALL_RPATH "@executable_path/../Frameworks"
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Frameworks"

            # COPY EACH DYLIB INTO THE BUNDLE
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/external/raylib-5.5_macos/lib/libraylib.5.5.0.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Frameworks/libraylib.5.5.0.dylib"

            # Recreate symlinks
            COMMAND ln -sf
            "libraylib.5.5.0.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Frameworks/libraylib.550.dylib"

            COMMAND ln -sf
            "libraylib.550.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Frameworks/libraylib.dylib"

            # Fix install name of the real dylib
            COMMAND install_name_tool
            -id "@rpath/libraylib.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Frameworks/libraylib.5.5.0.dylib"
    )
endif()


# Explicitly link win multimedia if building for win32
if (WIN32)
    target_link_libraries(cellular-automata raylib Winmm)
else ()
    target_link_libraries(cellular-automata raylib)
endif ()


# Set compile flags for WASM target during program linkage
if(EMSCRIPTEN)
    message("Passing WASM-specific linkage flags")
    target_link_options(
            cellular-automata
            PRIVATE
            -sALLOW_MEMORY_GROWTH=1
    )
endif()