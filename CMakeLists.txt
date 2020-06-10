cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME optp)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

option(CONAN_REFERENCE "Set the conan reference username/tag pair")
option(CONAN_PROFILE "Set the conan profile")

if(WIN32)
	message("Building for windows")
else()
	message("Building for other platforms")
	set(OTHER_LIBS pthread)
endif()

# === Installing conan dependencies ============================================
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
	file(COPY ${CMAKE_SOURCE_DIR}/conanfile.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	set(CMD)
	if(CONAN_REFERENCE)
		list(APPEND CMD "${CONAN_REFERENCE}")
	endif()
	if(CONAN_PROFILE)
		list(APPEND CMD "-pr")
		list(APPEND CMD "${CONAN_PROFILE}")
	endif()
	execute_process(
		COMMAND conan install . ${CMD}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		RESULT_VARIABLE CONAN_INSTALL_SUCCESS
	)
	if(NOT CONAN_INSTALL_SUCCESS EQUAL 0)
		return()
	endif()
endif()

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
# === Installing conan dependencies ============================================

include(GNUInstallDirs)

message(STATUS "Configuring the library")
list(APPEND CLASS_FILES
	operation
	optp
	optp_config
	real_node
	remote_node
	uuid_provider
)

message(VERBOSE "Class files to be compiled")
foreach(CLASS ${CLASS_FILES})
	message(VERBOSE "\t${CLASS_FILES}")
	list(APPEND SOURCE_FILES_LIST ${CLASS}.cpp)
	list(APPEND HEADER_FILES_LIST ${CLASS}.h)
endforeach()

file(GLOB INTERFACE_FOLDER "${CMAKE_SOURCE_DIR}/interface/*")
list(APPEND HEADER_FILES_LIST ${INTERFACE_FOLDER})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES_LIST} ${HEADER_FILES_LIST})
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
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
		$<INSTALL_INTERFACE:interface>
	PRIVATE
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

# 3th party libraries
#
#message(STATUS "Adding third party libraries")
#message("${CMAKE_CURRENT_BINARY_DIR}/sockpp")
#find_package(sockpp PATHS "${CMAKE_CURRENT_BINARY_DIR}/sockpp" REQUIRED)
#find_package(nlohmann_json PATHS "${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json" REQUIRED)
#
#if(WIN32)
#	set(SOCKPP_LIBRARY_NAME sockpp-static)
#else()
#	set(SOCKPP_LIBRARY_NAME sockpp)
#endif()
#
#target_link_libraries(${PROJECT_NAME}
#	PRIVATE
#		sockpp::${SOCKPP_LIBRARY_NAME}
#		nlohmann_json::nlohmann_json
#	PUBLIC ${OTHER_LIBS}
#)
#
#
## Install
#
#message(STATUS "Adding install targets")
#install(TARGETS ${PROJECT_NAME}
#	EXPORT "${PROJECT_NAME}Config"
#	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#)
#
#export(TARGETS ${PROJECT_NAME}
#	NAMESPACE ${PROJECT_NAME}
#	FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#)
#
#install(EXPORT
#	"${PROJECT_NAME}Config"
#	DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
#	NAMESPACE ${PROJECT_NAME}
#)
#
## Additional directories
#
#
## === Library configuration finished ===========================================
#
# === Test package configuration ===============================================
set(PROJET_TEST_NAME ${PROJECT_NAME}_test)
message(STATUS "Configuring the test executable")
list(APPEND TEST_CLASS_FILES
	fake_node
	interpreter
)

message(VERBOSE "Test classes")
foreach(TEST_CLASS ${TEST_CLASS_FILES})
	message(STATUS "\t${TEST_CLASS}")
	list(APPEND TEST_FILES_LIST "test/${TEST_CLASS}.cpp")
	list(APPEND TEST_FILES_LIST "test/${TEST_CLASS}.h")
endforeach()

message(STATUS "HEADERS\n\t${HEADER_FILES_LIST}")
message(STATUS "SOURCES\n\t${SOURCE_FILES_LIST}")

add_executable(${PROJET_TEST_NAME} "test/test.cpp" ${TEST_FILES_LIST} "test/test_config.json")
add_custom_command(TARGET ${PROJET_TEST_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/test/test_config.json" ${CMAKE_BINARY_DIR}
)

#get_target_property(SOCKPP_INCLUDE_DIR sockpp::${SOCKPP_LIBRARY_NAME} INTERFACE_INCLUDE_DIRECTORIES)
#get_target_property(SOCKPP_LIBRARY_DIR sockpp::sockpp-static IMPORTED_LOCATION_DEBUG)
#get_target_property(SOCKPP_LIBRARIES sockpp::sockpp-static INTERFACE_LINK_LIBRARIES)

target_include_directories(${PROJECT_NAME}_test
	PRIVATE ${CMAKE_SOURCE_DIR}
		${CMAKE_BINARY_DIR}
		${CONAN_INCLUDE_DIRS_SOCKPP}
		${CONAN_INCLUDE_DIRS_SPDLOG}
		${CONAN_INCLUDE_DIRS_NLOHMANN_JSON}
)
#target_link_directories(${PROJECT_NAME} PUBLIC ${SOCKPP_LIBRARY_DIR})

target_precompile_headers(${PROJECT_NAME}_test
	PRIVATE ${CONAN_INCLUDE_DIRS_NLOHMANN_JSON}/nlohmann/json.hpp
)

target_link_libraries(${PROJECT_NAME}_test
	PUBLIC optp
)

target_link_directories(${PROJECT_NAME}_test
PRIVATE
	${CONAN_LIB_DIRS_SOCKPP}
	${CONAN_LIB_DIRS_SPDLOG}
)

# === Test package configuration finished ======================================
