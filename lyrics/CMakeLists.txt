cmake_minimum_required(VERSION 3.17)

project(lyrics)

if (UNIX AND DEFINED ENV{CLION_IDE})
    #Giusto per avere l'autocompletamento funzionante da CLion
    include_directories($ENV{HOME}/emsdk/upstream/emscripten/system/include)
endif ()

# find files
file(GLOB lyrics_HEADERS static/*.h)
file(GLOB lyrics_SOURCES static/*.cpp)

# set up target
add_executable(lyrics ${lyrics_SOURCES} ${lyrics_HEADERS})

# target options
target_include_directories(lyrics PRIVATE fin)

target_link_libraries(lyrics fin)

target_compile_features(lyrics PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(lyrics PRIVATE /W4)
else ()
    target_compile_options(lyrics PRIVATE -Wall -Wextra)
endif ()

set_target_properties(lyrics
        PROPERTIES
        LINK_FLAGS "-s AUDIO_WORKLET=1 -s WASM_WORKERS=1 -s ALLOW_MEMORY_GROWTH=1 -sFETCH")
set(CMAKE_EXECUTABLE_SUFFIX ".html")