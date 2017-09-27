set(CMAKE_CXX_FLAGS "-Wall -std=c++1z ${CMAKE_CXX_FLAGS}")
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
add_test(
    NAME placeholder
    COMMAND $<TARGET_FILE:placeholder_test>
    )
