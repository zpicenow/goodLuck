cmake_minimum_required(VERSION 3.15)
project(goodLuck)

set(CMAKE_CXX_STANDARD 14)
link_directories(lib)

add_executable(goodLuck main.cpp)
target_link_libraries(goodLuck NeuronDataReader.dll)
target_link_libraries(goodLuck ws2_32.dll)