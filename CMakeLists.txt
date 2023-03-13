cmake_minimum_required(VERSION 3.17)
project(audio_fingerprint)

option(BUILD_CLIENT "Build the client side code" ON)
option(BUILD_SERVER "Build the server side code" OFF)

if(BUILD_CLIENT)
    add_definitions(-DBUILD_CLIENT)
endif()
if(BUILD_SERVER)
    add_definitions(-DBUILD_SERVER)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -mtune=native -flto")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")

add_subdirectory(mariadbpp)
include_directories(mariadbpp/include)

add_executable(audio_fingerprint
        main.cpp
        IO/Readers/WavReader.cpp IO/Readers/WavReader.h
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
        IO/DB.cpp IO/DB.h
        Utils/Wrapper.h IO/Readers/Reader.h wasm.cpp IO/Readers/DummyReader.cpp IO/Readers/DummyReader.h)

target_link_libraries(audio_fingerprint fftw3f mariadbclientpp stdc++fs)
target_compile_definitions(audio_fingerprint PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")