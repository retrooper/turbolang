cmake_minimum_required(VERSION 3.17)
project(turbolang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(include)

file(GLOB all_SRCS "${PROJECT_SOURCE_DIR}/cpp/*.cpp")
add_executable(turbolang ${all_SRCS})

find_package(LLVM REQUIRED)
target_link_libraries(turbolang LLVM)

