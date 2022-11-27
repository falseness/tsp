set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

cmake_minimum_required(VERSION 3.16)
project(tsp)

set(CMAKE_CXX_STANDARD 20)


include_directories(${PROJECT_SOURCE_DIR})

set(    GENERAL_SOURCE
        utils/graph.cpp
        utils/random.cpp
        solvers/original_solver.cpp
        )

set(    MAIN_SOURCE
        main.cpp
        ${GENERAL_SOURCE}
        )

#--- TESTS
set(EXTERNAL_INSTALL_LOCATION ${Testing_SOURCE_DIR}/3rdparty)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(Threads REQUIRED)

set(TEST_SOURCES
        tests/solver.cpp
        ${GENERAL_SOURCE} tests/solver.h)

add_executable(test ${TEST_SOURCES})

target_link_libraries(test gtest_main gtest Threads::Threads)
#---

add_executable(tsp ${MAIN_SOURCE})
