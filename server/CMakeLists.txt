cmake_minimum_required(VERSION 3.17)

project(server)

# find files
file(GLOB server_HEADERS *.h)
file(GLOB server_SOURCES *.cpp)

# Mariadbpp
add_subdirectory(libs/mariadbpp)
include_directories(libs/mariadbpp/include)

# set up target
add_executable(server ${server_SOURCES} ${server_HEADERS})

# target options
target_include_directories(server PRIVATE fin)

target_link_libraries(server fin mariadbclientpp stdc++fs)

target_compile_features(server PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(server PRIVATE /W4)
else ()
    target_compile_options(server PRIVATE -Wall -Wextra)
endif ()
