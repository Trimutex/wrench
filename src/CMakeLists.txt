find_package(Qt6 REQUIRED COMPONENTS Widgets)
add_executable(wrench
    ./main.cpp
    ./main_window.cpp
)

target_link_libraries(wrench PRIVATE Qt6::Widgets)
