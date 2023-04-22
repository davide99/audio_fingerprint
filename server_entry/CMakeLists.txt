cmake_minimum_required(VERSION 3.17)

project(server_entry)

# find files
set(server_entry_HEADERS)
set(server_entry_SOURCES main.cpp)

# set up target
add_executable(server_entry ${server_entry_SOURCES} ${server_entry_HEADERS})

# target options
target_include_directories(server_entry PRIVATE fin_db)

target_link_libraries(server_entry fin_db stdc++fs)

target_compile_features(server_entry PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(server_entry PRIVATE /W4)
else ()
    target_compile_options(server_entry PRIVATE -Wall -Wextra)
endif ()
