cmake_minimum_required(VERSION 3.6)

include(GNUInstallDirs)

message(STATUS "Configuring the ${PROJECT_NAME} library")
list(APPEND CLASS_FILES
	operation
	optp
	optp_config
	object
	real_node
	remote_node
	uuid_provider
	network_interfaces
	node_def
	operation_result
	operation_handler
)

message(VERBOSE "Class files to be compiled")
foreach(CLASS ${CLASS_FILES})
	message(VERBOSE "\t${CLASS_FILES}")
	list(APPEND SOURCE_FILES_LIST ${CLASS}.cpp)
	list(APPEND HEADER_FILES_LIST ${CLASS}.h)
endforeach()

file(GLOB INTERFACE_FOLDER "${CMAKE_SOURCE_DIR}/include/optp/*")
list(APPEND HEADER_FILES_LIST ${INTERFACE_FOLDER})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES_LIST} ${HEADER_FILES_LIST} "typedefs.h")
target_link_libraries(${PROJECT_NAME}
PRIVATE
	${CONAN_LIBS_SOCKPP}
	${CONAN_SYSTEM_LIBS_SOCKPP}
	${CONAN_LIBS_SPDLOG}
)

target_precompile_headers(${PROJECT_NAME}
	PRIVATE ${CONAN_INCLUDE_DIRS_NLOHMANN_JSON}/nlohmann/json.hpp
)

target_link_directories(${PROJECT_NAME}
PRIVATE
	${CONAN_LIB_DIRS_SOCKPP}
	${CONAN_LIB_DIRS_SPDLOG}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
		$<INSTALL_INTERFACE:interface>
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		${CONAN_INCLUDE_DIRS_SOCKPP}
		${CONAN_INCLUDE_DIRS_SPDLOG}
		${CONAN_INCLUDE_DIRS_NLOHMANN_JSON}
)

if (UNIX)
target_link_libraries(${PROJECT_NAME}
	PUBLIC pthread
)
endif()
# === Generate export headers ==================================================
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
	EXPORT_MACRO_NAME OPTP_EXPORT
	EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/${PROJECT_NAME}_export.h
)
# === Generate export headers ==================================================
