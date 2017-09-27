set (CMAKE_CXX_FLAGS "-std=c++1z ${CMAKE_CXX_FLAGS}")
include_directories(
    ../placeholder
    ${gtest_SOURCE_DIR}/include
    )
add_executable(
    placeholder_test
    placeholder.cpp
    )
target_link_libraries(
    placeholder_test
    gtest
    gtest_main
    )
