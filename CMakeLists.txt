cmake_minimum_required(VERSION 3.17)
project(turbolang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
include_directories(include)

file(GLOB all_SRCS "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(turbolang ${all_SRCS})

#Add LLVM
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
llvm_map_components_to_libnames(llvm_libs support core irreader)
message(STATUS "LLVM libs ${llvm_libs}")

target_link_libraries(turbolang ${llvm_libs})