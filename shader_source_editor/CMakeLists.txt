cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(shader_source_editor)

find_package(PkgConfig)
pkg_check_modules(PC_GTK3 QUIET gtk+-3.0)

set(GTK3_DEFINITIONS ${PC_GTK3_CFLAGS_OTHER})
set(GTK3_LIBRARY ${PC_GTK3_LIBRARIES} )
set(GTK3_INCLUDE_DIR ${PC_GTK3_INCLUDE_DIRS} )

set(GTK3_LIBRARIES ${GTK3_LIBRARY} )
set(GTK3_INCLUDE_DIRS ${GTK3_INCLUDE_DIR} )

include_directories(${GTK3_INCLUDE_DIRS})
add_definitions(${GTK3_DEFINITIONS})

file(GLOB shader_source_editor_cpp_header_files *.hpp)
file(GLOB shader_source_editor_cpp_files *.cpp)
file(GLOB shader_source_editor_c_header_files *.h)
file(GLOB shader_source_editor_c_files *.c)

add_executable(shader_source_editor WIN32
	${shader_source_editor_cpp_header_files}
	${shader_source_editor_cpp_files}
	${shader_source_editor_c_header_files}
	${shader_source_editor_c_files})

target_link_libraries(shader_source_editor elengine)
target_link_libraries(shader_source_editor ${GTK3_LIBRARIES})

