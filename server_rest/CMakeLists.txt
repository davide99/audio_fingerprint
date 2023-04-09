cmake_minimum_required(VERSION 3.17)

project(server_rest)

# find files
set(server_rest_HEADERS)
set(server_rest_SOURCES main.cpp)
add_subdirectory(libs/cpp-httplib)

# set up target
add_executable(server_rest ${server_rest_SOURCES} ${server_rest_HEADERS})

# target options
target_include_directories(server_rest PRIVATE fin fin_db cpp-httplib)

target_link_libraries(server_rest fin fin_db stdc++fs httplib::httplib)

target_compile_features(server_rest PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(server_rest PRIVATE /W4)
else ()
    target_compile_options(server_rest PRIVATE -Wall -Wextra)
endif ()
