cmake_minimum_required(VERSION 3.20)
project(testInvertedIndex)

set(CMAKE_CXX_STANDARD 17)

add_executable(testInvertedIndex testInvertedIndex.cpp
                                 ${MY_SOURCE_DIR}/inverted_index.cpp
                                 ${MY_INCLUDE_DIR}/inverted_index.h
                                 ${MY_SOURCE_DIR}/converter_json.cpp
                                 ${MY_INCLUDE_DIR}/converter_json.h
                                 )

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_Declare(
        thread-pool
        URL
        https://github.com/bshoshany/thread-pool/archive/refs/tags/v2.0.0.tar.gz
)
FetchContent_MakeAvailable(thread-pool)
target_include_directories(testInvertedIndex PRIVATE ${MY_INCLUDE_DIR} ${MY_SOURCE_DIR} ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${thread-pool_SOURCE_DIR})
set(gtest_disable_pthreads on)
FetchContent_MakeAvailable(googletest)
enable_testing()
target_link_libraries(testInvertedIndex PRIVATE gtest_main nlohmann_json::nlohmann_json)
include(GoogleTest)
gtest_discover_tests(testInvertedIndex)