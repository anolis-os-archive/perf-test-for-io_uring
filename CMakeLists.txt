cmake_minimum_required(VERSION 2.8)
project(perf-test-for-io_uring)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SRCS_TEST "src/test/*.cpp")
file(GLOB SRCS_LOADER "src/loader/*.cpp")

foreach (SRC_FILE_LOADER ${SRCS_LOADER})
    get_filename_component(LOADER_FILE ${SRC_FILE_LOADER} NAME)
    string(REPLACE ".cpp" "" LOADER_NAME ${LOADER_FILE})

    foreach (SRC_FILE_TEST ${SRCS_TEST})
        get_filename_component(TEST_FILE ${SRC_FILE_TEST} NAME)
        string(REPLACE ".cpp" "" TEST_NAME ${TEST_FILE})

        add_executable(${LOADER_NAME}_${TEST_NAME} ${SRC_FILE_TEST} ${SRC_FILE_LOADER} src/main.cpp)
        target_link_libraries(${LOADER_NAME}_${TEST_NAME} uring)
        target_link_libraries(${LOADER_NAME}_${TEST_NAME} pthread)
    endforeach (SRC_FILE_TEST ${SRCS_TEST})
endforeach (SRC_FILE_LOADER ${SRCS_LOADER})