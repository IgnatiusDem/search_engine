cmake_minimum_required(VERSION 3.20)
project(search_engine)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(nlohmann_json)
add_subdirectory(test)

add_executable(search_engine ${MY_INCLUDE_DIR}/inverted_index.cpp
                             ${MY_INCLUDE_DIR}/inverted_index.h
                             ${MY_INCLUDE_DIR}/converter_json.cpp
                             ${MY_INCLUDE_DIR}/converter_json.h
                             ${MY_INCLUDE_DIR}/search_server.cpp
                             ${MY_INCLUDE_DIR}/search_server.h
                             main.cpp)

target_include_directories(search_engine PRIVATE ${MY_INCLUDE_DIR})
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)




