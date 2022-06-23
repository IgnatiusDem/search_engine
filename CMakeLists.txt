cmake_minimum_required(VERSION 3.20)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

#set(JSON_BuildTests OFF CACHE INTERNAL "")

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(test)

include(FetchContent)
FetchContent_Declare(json
        URL
        https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

#include_directories(lib/thread-pool-2.0.0)
FetchContent_Declare(
        thread-pool
        URL
        https://github.com/bshoshany/thread-pool/archive/refs/tags/v2.0.0.tar.gz
)
FetchContent_MakeAvailable(thread-pool)

add_executable(search_engine main.cpp
                             ${MY_SOURCE_DIR}/inverted_index.cpp
                             ${MY_INCLUDE_DIR}/inverted_index.h
                             ${MY_SOURCE_DIR}/converter_json.cpp
                             ${MY_INCLUDE_DIR}/converter_json.h
                             ${MY_SOURCE_DIR}/search_server.cpp
                             ${MY_INCLUDE_DIR}/search_server.h
                             )

target_include_directories(search_engine PRIVATE ${MY_INCLUDE_DIR} ${MY_SOURCE_DIR} ${thread-pool_SOURCE_DIR})
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)




