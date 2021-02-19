# build this to generate a project for testing dynamic_array.h
cmake_minimum_required(VERSION "3.12")
project(dynamic_array_testing)

add_executable(dynm_arr_test "./testing/main.cpp" "./dynamic_array.h")

include_directories(dynm_arr_test "${CMAKE_CURRENT_SOURCE_DIR}")