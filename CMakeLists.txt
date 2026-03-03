cmake_minimum_required(VERSION 3.15)
project(C-Project)


if (MSVC)

   add_compile_options(/fsanitize=address /Zi)
    add_link_options(/fsanitize=address /INCREMENTAL:NO /DEBUG)

endif()


add_subdirectory(external/sfml)
add_executable(game src/main.cpp)
target_link_libraries(game sfml-graphics sfml-window sfml-system)