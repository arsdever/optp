cmake_minimum_required(VERSION 3.6)

project(operations VERSION 0.0.1 LANGUAGES CXX)

set(UNITS
	disconnect_op
	node_uuid_getter_op
)

foreach(UNIT ${UNITS})
	list(APPEND HEADERS ${UNIT}.h)
	list(APPEND SOURCES ${UNIT}.cpp)
endforeach()

add_library(operations STATIC ${HEADERS} ${SOURCES} "operation_types.h")
target_include_directories(operations PUBLIC 
	../
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/exports
	${CONAN_INCLUDE_DIRS_NLOHMANN_JSON}
)