cmake_minimum_required(VERSION 3.24)
project(WebServer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

add_executable(web_server src/tcp_server.cpp src/main.cpp)
target_include_directories(web_server PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(web_server PRIVATE -g -fsanitize=address,undefined)
    target_link_options( web_server PRIVATE -fsanitize=address,undefined)
endif()
target_compile_options(web_server PRIVATE -Wall -Wextra -Wpedantic -Wshadow)

target_link_libraries(web_server
        PRIVATE
        $<$<PLATFORM_ID:Windows>:ws2_32>
)
