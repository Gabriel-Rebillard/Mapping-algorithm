cmake_minimum_required(VERSION 3.1)         # Obligatoire
project(TP_CMake)                           # Obligatoire
set(CMAKE_CXX_STANDARD 17)                  # Utiliser C++17

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")

add_library(mapLib src/mapReader.cpp src/mapGenerator.cpp src/map.cpp)
add_library(graphLib src/graph.cpp)
add_library(robotLib src/robot.cpp)
add_library(statisticsLib src/statistics.cpp)
add_executable(main src/main.cpp)
target_link_libraries(main mapLib robotLib statisticsLib graphLib)
