cmake_minimum_required(VERSION 3.20)

project(cflat)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(include EXCLUDE_FROM_ALL)
find_package(fmt REQUIRED)

set_target_properties(fmt PROPERTIES LINKER_LANGUAGE C)

add_executable(cf src/main.cpp)

target_link_libraries(cf PRIVATE fmt::fmt-header-only)