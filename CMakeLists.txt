cmake_minimum_required(VERSION 3.20)
project(search_engine)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(nlohmann_json)

add_executable(search_engine inverted_index.cpp ${MY_INCLUDE_DIR}/inverted_index.h converter_json.cpp ${MY_INCLUDE_DIR}/converter_json.h search_server.cpp ${MY_INCLUDE_DIR}/search_server.h main.cpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
FetchContent_MakeAvailable(googletest)
enable_testing()
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json gtest_main)
include(GoogleTest)
gtest_discover_tests(search_engine)



