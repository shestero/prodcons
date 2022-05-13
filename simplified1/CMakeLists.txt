cmake_minimum_required(VERSION 3.20)

project(prodcons)
add_executable(${PROJECT_NAME} "main.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
# target_include_directories(${PROJECT_NAME} PRIVATE ".")

SET(THREADS_PREFER_PTHREAD_FLAG ON)

# add_compile_options(-pthread)
# add_compile_options(-std=c++17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")

set_property(TARGET prodcons PROPERTY CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD 17)

