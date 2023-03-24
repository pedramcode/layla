cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
project(layla LANGUAGES CXX VERSION 0.0.1)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE xxhash)