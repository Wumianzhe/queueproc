cmake_minimum_required(VERSION 3.12)
project(queueproc)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
if (DEFINED build_tests)
  add_subdirectory(tests)
endif()
add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC queueproc)
