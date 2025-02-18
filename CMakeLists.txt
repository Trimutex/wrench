cmake_minimum_required(VERSION 3.10)

project(
    wrench
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SRCFILES "src/*.cpp")
add_executable(wrench ${SRCFILES})

target_link_libraries(wrench PRIVATE Qt6::Widgets)
install(TARGETS wrench)
