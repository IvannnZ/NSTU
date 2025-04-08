cmake_minimum_required(VERSION 3.10)
project(RGZ)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 REQUIRED graphics window system)

add_executable(main main.cpp)
target_link_libraries(main sfml-graphics sfml-window sfml-system pthread)
