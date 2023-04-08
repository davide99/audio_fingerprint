cmake_minimum_required(VERSION 3.17)

project(fin_db LANGUAGES CXX)

# find files
set(fin_db_HEADERS
        include/fin_db/db.h
        include/fin_db/utils.h
        include/fin_db/fin_db.h
        )
set(fin_db_SOURCES
        src/db.cpp
        src/utils.cpp)

# Mariadbpp
add_subdirectory(libs/mariadbpp)
include_directories(libs/mariadbpp/include)

# set up target
add_library(fin_db ${fin_db_SOURCES} ${fin_db_HEADERS})

# target options
target_include_directories(
        fin_db
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        fin
)

target_link_libraries(fin_db fin mariadbclientpp)

target_compile_features(fin_db PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(fin_db PRIVATE /W4)
else ()
    target_compile_options(fin_db PRIVATE -Wall -Wextra)
endif ()
