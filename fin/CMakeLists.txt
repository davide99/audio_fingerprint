cmake_minimum_required(VERSION 3.17)

project(fin LANGUAGES CXX)

# find files
set(fin_HEADERS
        include/fin/core/fingerprint.h
        include/fin/core/link.h include/fin/core/links.h
        include/fin/core/peak.h include/fin/core/peaks_finder.h

        include/fin/math/fft_window.h
        include/fin/math/integers.h
        include/fin/math/spectrogram.h
        include/fin/math/vector.h
        include/fin/math/window.h

        include/fin/readers/reader.h
        include/fin/readers/wav_reader.h
        include/fin/readers/dummy_reader.h

        include/fin/utils/fixed_size_pq.h
        include/fin/utils/utils.h

        include/fin/fin.h
        )
set(fin_SOURCES
        src/core/fingerprint.cpp
        src/core/link.cpp src/core/links.cpp
        src/core/peak.cpp src/core/peaks_finder.cpp

        src/math/integers.cpp
        src/math/spectrogram.cpp
        src/math/window.cpp

        src/readers/wav_reader.cpp
        src/readers/dummy_reader.cpp

        src/utils/utils.cpp
        src/fin.cpp)

# set up target
add_library(fin ${fin_SOURCES} ${fin_HEADERS})

#fftw
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0075 NEW)

set(ENABLE_FLOAT ON)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_TESTS OFF)
set(DISABLE_FORTRAN ON)
set(ENABLE_THREADS ON)
add_subdirectory(libs/fftw)

# target options
target_link_libraries(fin fftw3f)

target_include_directories(
        fin
        PUBLIC
        libs/fftw/api libs/xxHash
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(fin PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(fin PRIVATE /W4)
else ()
    target_compile_options(fin PRIVATE -Wall -Wextra)
endif ()
