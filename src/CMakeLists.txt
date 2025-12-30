cmake_minimum_required(VERSION 3.20)
project(FTMP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# Dependencies
# -----------------------------
include(FetchContent)

# FTXUI
FetchContent_Declare(
        ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
        GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)

# SDL2 (for audio handling)
# NixOS: point pkg-config to SDL2 .pc file in the store so pkg_check_modules can find it.
set(ENV{PKG_CONFIG_PATH} "/nix/store/zrk9hppfabwygs5nmqcwn9xmlw93zbj3-sdl2-compat-2.32.58-dev/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# SDL2_mixer (for audio mixing)
# Detected via pkg-config as "SDL2_mixer" once its dev package is available.
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

# -----------------------------
# Define the main executable
# -----------------------------
add_executable(ftmp main.cpp)

# -----------------------------
# Link FTXUI, SDL2, and SDL2_mixer
# -----------------------------
# Include paths
target_include_directories(ftmp PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
)

# Libraries
target_link_libraries(ftmp
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        PRIVATE ${SDL2_LIBRARIES}
        PRIVATE ${SDL2_MIXER_LIBRARIES}
)

target_compile_options(ftmp PRIVATE
        ${SDL2_CFLAGS_OTHER}
        ${SDL2_MIXER_CFLAGS_OTHER}
)
