cmake_minimum_required(VERSION 3.15)
project(C-Project)


if (MSVC)

   add_compile_options(/fsanitize=address /Zi)
    add_link_options(/fsanitize=address /INCREMENTAL:NO /DEBUG)

endif()


add_subdirectory(external/sfml)
add_executable(game src/main.cpp)
target_link_libraries(game sfml-graphics sfml-window sfml-system sfml-audio)
target_compile_features(game PRIVATE cxx_std_23)
target_compile_definitions(game PRIVATE ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")