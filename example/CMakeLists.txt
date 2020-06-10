cmake_minimum_required(VERSION 3.6)

set(EXAMPLE_NAME ${PROJECT_NAME}_example)

message(STATUS "Configuring the example executable")
list(APPEND EXAMPLE_CLASS_FILES
	fake_node
	interpreter
)

message(VERBOSE "Example classes")
foreach(EXAMPLE_CLASS ${EXAMPLE_CLASS_FILES})
	message(STATUS "\t${EXAMPLE_CLASS}")
	list(APPEND EXAMPLE_FILES_LIST "${EXAMPLE_CLASS}.cpp")
	list(APPEND EXAMPLE_FILES_LIST "${EXAMPLE_CLASS}.h")
endforeach()

add_executable(${EXAMPLE_NAME} "example.cpp" ${EXAMPLE_FILES_LIST})
add_custom_command(TARGET ${EXAMPLE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/test_config.json" ${CMAKE_BINARY_DIR}
)

target_include_directories(${PROJECT_NAME}_example
	PRIVATE ${CMAKE_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/src
		${CMAKE_BINARY_DIR}
		${CONAN_INCLUDE_DIRS_SOCKPP}
		${CONAN_INCLUDE_DIRS_SPDLOG}
		${CONAN_INCLUDE_DIRS_NLOHMANN_JSON}
)

target_precompile_headers(${PROJECT_NAME}_example
	PRIVATE ${CONAN_INCLUDE_DIRS_NLOHMANN_JSON}/nlohmann/json.hpp
)

target_link_libraries(${PROJECT_NAME}_example
	PUBLIC optp
)

target_link_directories(${PROJECT_NAME}_example
PRIVATE
	${CONAN_LIB_DIRS_SOCKPP}
	${CONAN_LIB_DIRS_SPDLOG}
)