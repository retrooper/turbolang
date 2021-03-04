cmake_minimum_required(VERSION 3.17)
project(turbolang)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_executable(turbolang main.cpp cpp/timeutil.cpp cpp/printer.cpp cpp/compilermanager.cpp cpp/objectwrapper.cpp cpp/tokenizer.cpp cpp/parser.cpp cpp/sleeper.cpp cpp/exiter.cpp cpp/functiondefinition.cpp cpp/type.cpp cpp/statement.cpp cpp/functioncallprocessor.cpp)

find_package(LLVM REQUIRED)
target_link_libraries(turbolang LLVM)

