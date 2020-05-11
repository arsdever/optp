cmake_minimum_required(VERSION 3.6)
project(optp VERSION 0.0.1 LANGUAGES CXX)

add_library(optp SHARED
	optp.cpp
	optp_config.cpp)

add_executable(optp_test test.cpp)
target_link_libraries(optp PUBLIC sockpp json spdlog)
target_link_libraries(optp_test PUBLIC optp)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/sockpp/include)
include_directories(${CMAKE_SOURCE_DIR}/json/include)
include_directories(${CMAKE_SOURCE_DIR}/spdlog/include)
