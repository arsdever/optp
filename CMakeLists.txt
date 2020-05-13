cmake_minimum_required(VERSION 3.6)
project(optp VERSION 0.0.1 LANGUAGES CXX)

add_library(optp SHARED
	optp.cpp
	optp_config.cpp)

add_executable(optp_test test.cpp)

find_library(SOCKPP_LIB sockpp PATHS ${CMAKE_SOURCE_DIR}/sockpp/build_linux REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${sockpp_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/json/include)
include_directories(${CMAKE_SOURCE_DIR}/spdlog/include)
target_link_libraries(optp PUBLIC ${SOCKPP_LIB})
target_link_libraries(optp_test PUBLIC optp pthread)
