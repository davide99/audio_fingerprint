cmake_minimum_required(VERSION 3.17)
project(audio_fingerprint)

option(JUST_WASM "Build only wasm projects" OFF)

add_subdirectory(fin)
add_subdirectory(wasm)
add_subdirectory(lyrics)
if (NOT JUST_WASM)
    add_subdirectory(fin_db)
    add_subdirectory(server_entry)
    add_subdirectory(server_rest)
    add_subdirectory(mock_client)
endif ()
