cmake_minimum_required(VERSION 3.20)

project(ttaskconsprod)
add_executable(${PROJECT_NAME} "main.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
# target_include_directories(${PROJECT_NAME} PRIVATE ".")

SET(THREADS_PREFER_PTHREAD_FLAG ON)

# add_compile_options(-pthread)
# add_compile_options(-std=c++20)
# add_compile_options(-std=c++2b)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++2b")

set_property(TARGET ttaskconsprod PROPERTY CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD 23)

