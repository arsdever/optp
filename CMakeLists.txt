cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME optp)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)


if(WIN32)
else()
	set(OTHER_LIBS pthread)
endif()

# === Library configuration ================================================================

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

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES_LIST} ${HEADER_FILES_LIST})

# 3th party libraries

message(STATUS "Adding third party libraries")
find_package(sockpp PATHS "${CMAKE_SOURCE_DIR}/sockpp/build" REQUIRED)
find_package(nlohmann_json PATHS "${CMAKE_SOURCE_DIR}/json/build" REQUIRED)

if(WIN32)
	set(SOCKPP_LIBRARY_NAME sockpp-static)
else()
	set(SOCKPP_LIBRARY_NAME sockpp)
endif()

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		sockpp::${SOCKPP_LIBRARY_NAME}
		nlohmann_json::nlohmann_json
		${SOCKPP_LIBRARIES}
)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
	EXPORT_MACRO_NAME OPTP_EXPORT
	EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/${PROJECT_NAME}_export.h
)

# Install

message(STATUS "Adding install targets")
install(TARGETS ${PROJECT_NAME}
	EXPORT "${PROJECT_NAME}Config"
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

export(TARGETS ${PROJECT_NAME}
	NAMESPACE ${PROJECT_NAME}
	FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
)

install(EXPORT
	"${PROJECT_NAME}Config"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
	NAMESPACE ${PROJECT_NAME}
)

# Additional directories

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
		$<INSTALL_INTERFACE:interface>
	PRIVATE
		${CMAKE_SOURCE_DIR}/spdlog/include
)

# === Library configuration finished =======================================================

# === Test package configuration ===========================================================
set(PROJET_TEST_NAME ${PROJECT_NAME}_test)
message(STATUS "Configuring the test executable")
list(APPEND TEST_CLASS_FILES
	fake_node)


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

get_target_property(SOCKPP_INCLUDE_DIR sockpp::${SOCKPP_LIBRARY_NAME} INTERFACE_INCLUDE_DIRECTORIES)
#get_target_property(SOCKPP_LIBRARY_DIR sockpp::sockpp-static IMPORTED_LOCATION_DEBUG)
#get_target_property(SOCKPP_LIBRARIES sockpp::sockpp-static INTERFACE_LINK_LIBRARIES)

target_include_directories(${PROJECT_NAME}_test
	PRIVATE ${CMAKE_SOURCE_DIR}
		${CMAKE_BINARY_DIR}
		${CMAKE_SOURCE_DIR}/spdlog/include
	PUBLIC ${SOCKPP_INCLUDE_DIR}
)
#target_link_directories(${PROJECT_NAME} PUBLIC ${SOCKPP_LIBRARY_DIR})


target_link_libraries(${PROJECT_NAME}_test
	PUBLIC
		optp
		nlohmann_json::nlohmann_json
		${OTHER_LIBS}
		sockpp::${SOCKPP_LIBRARY_NAME})

# === Test package configuration finished ==================================================
