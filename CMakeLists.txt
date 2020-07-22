cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME optp)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

option(CONAN_REFERENCE "Set the conan reference username/tag pair")
option(CONAN_PROFILE "Set the conan profile")

if(WIN32)
	message("Building for windows")
else()
	message("Building for other platforms")
	set(OTHER_LIBS pthread)
endif()

#file(GLOB OBJECT_METATYPES_HEADERS "include/optp/*.h")
#set(METATYPE_IGNORE_LIST "object_metatypes;typedefs;deserializable;serializable")
#foreach(HEADER ${OBJECT_METATYPES_HEADERS})
#	get_filename_component(fname ${HEADER} NAME_WE)
#	list(FIND METATYPE_IGNORE_LIST ${fname} _index)
#	if (NOT ${_index} GREATER -1)
#		string(TOUPPER ${fname} FNAME)
#		list(APPEND OBJECT_METATYPES_LIST ${FNAME})
#		string(APPEND FWD_DECL "\t\tclass ${fname};\n")
#		string(APPEND PARTIAL_IMPLEMENTATIONS "\n\n\ttemplate\ <>\n\tstruct\ object_type<object_metatype::${FNAME}>\n\t{\n\t\ttypedef\ interfaces::${fname}\ type;\n\t};")
#	endif()
#endforeach()
set(METATYPE_LIST "operation;node;node_def;operation_result")
foreach(fname ${METATYPE_LIST})
	string(TOUPPER ${fname} FNAME)
	list(APPEND OBJECT_METATYPES_LIST ${FNAME})
	list(APPEND FWD_DECLARE_CLASSES  "class ${FNAME};")
endforeach()
string(JOIN ",\n\t\t" OBJECT_METATYPES ${OBJECT_METATYPES_LIST})
configure_file("${CMAKE_SOURCE_DIR}/cmake/object_metatypes.h.in"
	"${CMAKE_SOURCE_DIR}/include/optp/object_metatypes.h"
	@ONLY)

# === Installing conan dependencies ============================================
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
	file(COPY ${CMAKE_SOURCE_DIR}/conanfile.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	set(CMD)
	if(CONAN_REFERENCE)
		list(APPEND CMD "${CONAN_REFERENCE}")
	endif()
	if(CONAN_PROFILE)
		list(APPEND CMD "-pr")
		list(APPEND CMD "${CMAKE_SOURCE_DIR}${CONAN_PROFILE}")
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
