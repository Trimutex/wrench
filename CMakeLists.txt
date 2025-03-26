cmake_minimum_required(VERSION 3.10)

project(
    wrench
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SRCFILES "src/*.cpp")
add_executable(wrench ${SRCFILES})

target_link_libraries(wrench PRIVATE Qt6::Widgets)

string(JOIN "\n" WRENCH_DESKTOP
    "[Desktop Entry]"
    "Type=Application"
    "Terminal=false"
    "Name=Wrench"
    "GenericName=Wrench"
    "Comment=Linux Configuration App"
    "Exec=wrench"
)
file(GENERATE OUTPUT wrench.desktop CONTENT "${WRENCH_DESKTOP}")

install(TARGETS wrench)
install(FILES "${CMAKE_BINARY_DIR}/wrench.desktop" DESTINATION share/applications)
