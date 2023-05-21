cmake_minimum_required(VERSION 3.17)

project(wasm_client)

if (UNIX AND DEFINED ENV{CLION_IDE})
    #Giusto per avere l'autocompletamento funzionante da CLion
    include_directories($ENV{HOME}/emsdk/upstream/emscripten/system/include)
endif ()

# find files
file(GLOB wasm_HEADERS static/*.h)
file(GLOB wasm_SOURCES static/*.cpp)

# set up target
add_executable(wasm_client ${wasm_SOURCES} ${wasm_HEADERS})

# target options
target_include_directories(wasm_client PRIVATE fin)

target_link_libraries(wasm_client fin)

target_compile_features(wasm_client PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(wasm_client PRIVATE /W4)
else ()
    target_compile_options(wasm_client PRIVATE -Wall -Wextra)
endif ()

set_target_properties(wasm_client
        PROPERTIES
        LINK_FLAGS "-s AUDIO_WORKLET=1 -s WASM_WORKERS=1 -s ALLOW_MEMORY_GROWTH=1")
set(CMAKE_EXECUTABLE_SUFFIX ".html")