cmake_minimum_required(VERSION 3.6)

set(EXAMPLE_NAME ${PROJECT_NAME}_example)

message(STATUS "Configuring the example executable")
list(APPEND EXAMPLE_CLASS_FILES
	ping_operation
	ping_operation_handler
	interpreter
)

message(VERBOSE "Example classes")
foreach(EXAMPLE_CLASS ${EXAMPLE_CLASS_FILES})
	message(STATUS "\t${EXAMPLE_CLASS}")
	list(APPEND EXAMPLE_FILES_LIST "${EXAMPLE_CLASS}.cpp")
	list(APPEND EXAMPLE_FILES_LIST "${EXAMPLE_CLASS}.h")
endforeach()

add_executable(${EXAMPLE_NAME} "example.cpp" ${EXAMPLE_FILES_LIST} "ping_operation_result.h")
add_custom_command(TARGET ${EXAMPLE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/test_config.json" ${CMAKE_BINARY_DIR}
)

target_include_directories(${EXAMPLE_NAME}
	PRIVATE ${CMAKE_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/src
		${CMAKE_BINARY_DIR}
		${CONAN_INCLUDE_DIRS}
)

target_precompile_headers(${EXAMPLE_NAME}
	PRIVATE ../src/pch.h
)

target_link_libraries(${EXAMPLE_NAME}
	PUBLIC optp
)

target_link_directories(${EXAMPLE_NAME}
PRIVATE
	${CONAN_LIB_DIRS}
)