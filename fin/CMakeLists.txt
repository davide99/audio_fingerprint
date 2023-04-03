cmake_minimum_required(VERSION 3.17)

project(fin LANGUAGES CXX)

# find files
file(GLOB_RECURSE fin_HEADERS include/*.h)
file(GLOB_RECURSE fin_SOURCES src/*.cpp)

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
        libs/fftw/api libs/xxHash)

target_compile_features(fin PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(fin PRIVATE /W4)
else()
    target_compile_options(fin PRIVATE -Wall -Wextra)
endif()
