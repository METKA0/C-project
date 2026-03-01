cmake_minimum_required(VERSION 3.15)
project(C-Project)

add_subdirectory(external/sfml)
add_executable(game src/main.cpp)
target_link_libraries(game sfml-graphics sfml-window sfml-system)

if (MSVC)

    target_compile_options(game PRIVATE /fsanitize=address /Zi)

    target_link_options(game PRIVATE /INCREMENTAL:NO /DEBUG)

endif()