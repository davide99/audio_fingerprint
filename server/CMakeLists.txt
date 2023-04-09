cmake_minimum_required(VERSION 3.17)

project(server)

# find files
set(server_HEADERS)
set(server_SOURCES main.cpp)

# set up target
add_executable(server ${server_SOURCES} ${server_HEADERS})

# target options
target_include_directories(server PRIVATE fin fin_db)

target_link_libraries(server fin fin_db stdc++fs)

target_compile_features(server PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(server PRIVATE /W4)
else ()
    target_compile_options(server PRIVATE -Wall -Wextra)
endif ()
