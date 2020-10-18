cmake_minimum_required(VERSION 3.6)

include(GNUInstallDirs)

message(STATUS "Configuring the ${PROJECT_NAME} library")
list(APPEND CLASS_FILES
	optp
	object
	node_def
	real_node
	operation
	optp_config
	remote_node
	uuid_provider
	operation_result
	operation_handler
	network_interfaces
	connector
)

set(METATYPE_LIST "operation;node_def;operation_result")
foreach(fname ${METATYPE_LIST})
	string(TOUPPER ${fname} FNAME)
	list(APPEND OBJECT_METATYPES_LIST_U "${FNAME} = object_type_id<${fname}>::value")
	list(APPEND OBJECT_METATYPES_LIST_L ${fname})
	string(APPEND FWD_DECLARE_CLASSES "\n\tclass ${fname};")
endforeach()
string(JOIN ",\n\t\t\t" OBJECT_METATYPES_UPPER_LIST ${OBJECT_METATYPES_LIST_U})
string(JOIN ",\n\t\t\t" OBJECT_METATYPES_LOWER_LIST ${OBJECT_METATYPES_LIST_L})
#string(JOIN "\n\t" FWD_DECLARE_CLASSES ${FWD_DECLARE_CLASSES_})
configure_file("${CMAKE_SOURCE_DIR}/cmake/object_metatypes.h.in"
	"${CMAKE_SOURCE_DIR}/include/optp/object_metatypes.h"
	@ONLY)

message(VERBOSE "Class files to be compiled")
foreach(CLASS ${CLASS_FILES})
	message(VERBOSE "\t${CLASS_FILES}")
	list(APPEND SOURCE_FILES_LIST ${CLASS}.cpp)
	list(APPEND HEADER_FILES_LIST ${CLASS}.h)
endforeach()

file(GLOB INTERFACE_FOLDER "${CMAKE_SOURCE_DIR}/include/optp/*")
list(APPEND HEADER_FILES_LIST ${INTERFACE_FOLDER})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES_LIST} ${HEADER_FILES_LIST} "typedefs.h" "pch.h")

target_link_libraries(${PROJECT_NAME}
PRIVATE
	${CONAN_LIBS}
	${CONAN_SYSTEM_LIBS}
	${OTHER_LIBS}
)

target_precompile_headers(${PROJECT_NAME}
	PRIVATE pch.h
)

target_link_directories(${PROJECT_NAME}
PRIVATE
	${CONAN_LIB_DIRS}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
		$<INSTALL_INTERFACE:interface>
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		${CONAN_INCLUDE_DIRS}
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

add_subdirectory(operations)
