cmake_minimum_required( VERSION 3.7 )
project(time_point)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif(MSVC)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)


FILE(GLOB_RECURSE SRC_LISTS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(time_point ${SRC_LISTS})

add_executable(time_test ${CMAKE_SOURCE_DIR}/test/time_test.cpp)
target_link_libraries(time_test PRIVATE time_point)
