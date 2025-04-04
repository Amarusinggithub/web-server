cmake_minimum_required(VERSION 3.10)
project(WebServer)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_executable(web_server src/tcp_server.cpp src/server.cpp)


include_directories(${ws2_32_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(web_server ws2_32)
