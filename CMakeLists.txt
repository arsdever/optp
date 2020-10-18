cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME optp)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

#set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

option(CONAN_REFERENCE "Set the conan reference username/tag pair")
option(CONAN_PROFILE "Set the conan profile")

if(WIN32)
	message("Building for windows")
	set(OTHER_LIBS Iphlpapi)
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
		list(APPEND CMD "${CMAKE_SOURCE_DIR}/${CONAN_PROFILE}")
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

add_subdirectory(src)
add_subdirectory(example)
