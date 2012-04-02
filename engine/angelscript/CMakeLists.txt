project(angelscript)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# project options
set(${PROJECT_NAME}_MAJOR_VERSION 2)
set(${PROJECT_NAME}_MINOR_VERSION 23)
set(${PROJECT_NAME}_PATCH_LEVEL 0)

math(EXPR ${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION} * 10000 + ${${PROJECT_NAME}_MINOR_VERSION} * 100 + ${${PROJECT_NAME}_PATCH_LEVEL}")
set(${PROJECT_NAME}_VERSION_STRING "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}")

file(GLOB angelscript_header_files source/*.h)
file(GLOB angelscript_source_files source/*.cpp)

#include dir
file(GLOB include_header_files include/*.h)
list(APPEND angelscript_source_files ${include_header_files})

#scriptarray dir
file(GLOB scriptarray_header_files scriptarray/*.h)
file(GLOB scriptarray_source_files scriptarray/*.cpp)
list(APPEND angelscript_header_files ${scriptarray_source_files})
list(APPEND angelscript_source_files ${scriptarray_header_files})

#scriptbuilder dir
file(GLOB scriptbuilder_header_files scriptbuilder/*.h)
file(GLOB scriptbuilder_source_files scriptbuilder/*.cpp)
list(APPEND angelscript_header_files ${scriptbuilder_source_files})
list(APPEND angelscript_source_files ${scriptbuilder_header_files})

#scriptstdstring dir
file(GLOB scriptstdstring_header_files scriptstdstring/*.h)
file(GLOB scriptstdstring_source_files scriptstdstring/*.cpp)
list(APPEND angelscript_header_files ${scriptstdstring_source_files})
list(APPEND angelscript_source_files ${scriptstdstring_header_files})

add_executable(buildasglm scriptglm/buildasglm.cpp)

add_custom_command(OUTPUT scriptglm.cpp COMMAND buildasglm > ${CMAKE_CURRENT_BINARY_DIR}/scriptglm.cpp DEPENDS buildasglm ${CMAKE_CURRENT_SOURCE_DIR}/scriptglm/buildasglm.cpp)

list(APPEND angelscript_header_files scriptglm/scriptglm.h)
list(APPEND angelscript_source_files ${CMAKE_CURRENT_BINARY_DIR}/scriptglm.cpp)

add_definitions("-D_CRT_SECURE_NO_WARNINGS -DANGELSCRIPT_EXPORT -D_LIB")

CHECK_CXX_COMPILER_FLAG(-fno-strict-aliasing no_strict_aliasing)

add_library(angelscript ${angelscript_source_files} ${angelscript_header_files})

IF (${no_strict_aliasing})
	set_target_properties(angelscript PROPERTIES COMPILE_FLAGS "-fno-strict-aliasing")
ENDIF (${no_strict_aliasing})

set_target_properties(angelscript PROPERTIES VERSION ${${PROJECT_NAME}_VERSION_STRING})
