cmake_minimum_required(VERSION 3.17)
project(audio_fingerprint)

option(BUILD_CLIENT "Build the client side code" OFF)
option(BUILD_MOCK_CLIENT "Build the mock client side code" ON)
option(BUILD_SERVER "Build the server side code" OFF)

if (BUILD_CLIENT)
    message("Building WASM client")
    add_definitions(-DBUILD_CLIENT)
endif ()
if (BUILD_MOCK_CLIENT)
    message("Building mock client")
    add_definitions(-DBUILD_MOCK_CLIENT)
endif ()
if (BUILD_SERVER)
    message("Building server")
    add_definitions(-DBUILD_SERVER)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -mtune=native -flto")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")

if (BUILD_CLIENT)
    set(CMAKE_CXX_FLAGS "-fsanitize=undefined -Wcast-align")

    if (UNIX AND DEFINED ENV{CLION_IDE})
        #Giusto per avere l'autocompletamento funzionante da CLion
        include_directories($ENV{HOME}/emsdk/upstream/emscripten/system/include)
    endif ()
endif ()

# Mariadbpp
if (BUILD_SERVER)
    add_subdirectory(libs/mariadbpp)
    include_directories(libs/mariadbpp/include)
endif ()

#fftw
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0075 NEW)

set(ENABLE_FLOAT ON)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_TESTS OFF)
set(DISABLE_FORTRAN ON)
set(ENABLE_THREADS ON)
add_subdirectory(libs/fftw)

#common source files
set(SOURCE_FILES
        Utils/Utils.cpp Utils/Utils.h
        Consts.h
        Math/Integers.cpp Math/Integers.h
        Utils/FixedSizePQ.h
        Math/Window.h Math/Window.cpp
        Math/Vector.h
        Math/Spectrogram.cpp Math/Spectrogram.h
        Core/Fingerprint.cpp Core/Fingerprint.h
        Core/Peak.cpp Core/Peak.h
        Math/FFTWindow.h
        Core/PeaksFinder.cpp Core/PeaksFinder.h
        Core/Link.h Core/Link.cpp
        Core/Links.cpp Core/Links.h
        Utils/Wrapper.h
        IO/Readers/Reader.h)

if (BUILD_CLIENT)
    set(SOURCE_FILES
            ${SOURCE_FILES}
            wasm.cpp
            IO/Readers/DummyReader.cpp IO/Readers/DummyReader.h)
endif ()

if (BUILD_MOCK_CLIENT)
    set(SOURCE_FILES
            ${SOURCE_FILES}
            wasm_mock.cpp
            IO/Readers/WavReader.cpp IO/Readers/WavReader.h)
endif ()

if (BUILD_SERVER)
    set(SOURCE_FILES
            ${SOURCE_FILES}
            server.cpp
            IO/Readers/WavReader.cpp IO/Readers/WavReader.h
            IO/DB.cpp IO/DB.h)
endif ()

add_executable(audio_fingerprint ${SOURCE_FILES})

target_include_directories(audio_fingerprint PRIVATE libs/fftw/api libs/xxHash)

if (BUILD_SERVER)
    target_link_libraries(audio_fingerprint fftw3f mariadbclientpp stdc++fs)
else ()
    target_link_libraries(audio_fingerprint fftw3f)
endif ()

target_compile_definitions(audio_fingerprint PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
if (BUILD_CLIENT)
    set_target_properties(audio_fingerprint
            PROPERTIES
            LINK_FLAGS "-s AUDIO_WORKLET=1 -s WASM_WORKERS=1 -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()