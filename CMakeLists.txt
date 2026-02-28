cmake_minimum_required(VERSION 3.15) # add_link_options için 3.15+ daha iyidir
project(CProject)


add_executable(cproject main.cpp)


if(MSVC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    target_compile_options(cproject PUBLIC /fsanitize=address /Zi)
    

    target_link_options(cproject PUBLIC /fsanitize=address /INCREMENTAL:NO)
endif()