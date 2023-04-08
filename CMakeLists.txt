cmake_minimum_required(VERSION 3.17)
project(audio_fingerprint)

option(JUST_WASM "Build only wasm client" OFF)

add_subdirectory(fin)
add_subdirectory(wasm)
if (NOT JUST_WASM)
    add_subdirectory(fin_db)
    add_subdirectory(server)
    add_subdirectory(mock_client)
endif ()
