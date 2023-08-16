cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(HelloSFML)

## If you want to link SFML statically
#set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
add_executable(HelloSFML main.cpp Classes.h Classes.cpp Methods.cpp)
target_link_libraries(HelloSFML sfml-system sfml-window sfml-graphics)