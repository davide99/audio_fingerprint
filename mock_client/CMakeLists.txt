cmake_minimum_required(VERSION 3.17)

project(mock_client)

# set up target
add_executable(mock_client main.cpp)

# target options
target_include_directories(mock_client PRIVATE fin)

target_link_libraries(mock_client fin)

target_compile_features(mock_client PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(mock_client PRIVATE /W4)
else ()
    target_compile_options(mock_client PRIVATE -Wall -Wextra)
endif ()
