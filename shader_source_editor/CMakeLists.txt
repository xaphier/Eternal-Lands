cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(shader_source_editor)

find_package(PkgConfig)

if (PKG_CONFIG_FOUND)
	pkg_check_modules(PC_GTK QUIET gtk+-3.0)

	set(GTK_DEFINITIONS ${PC_GTK_CFLAGS_OTHER})
	set(GTK_LIBRARIES ${PC_GTK_LIBRARIES})
	set(GTK_INCLUDE_DIRS ${PC_GTK_INCLUDE_DIRS})
else (PKG_CONFIG_FOUND)
	include(FindGTK2)

	set(GTK_DEFINITIONS)
	set(GTK_LIBRARIES ${GTK2_LIBRARIES})
	set(GTK_INCLUDE_DIR ${GTK2_INCLUDE_DIRS})
endif (PKG_CONFIG_FOUND)

include_directories(${GTK_INCLUDE_DIRS})
add_definitions(${GTK_DEFINITIONS})

file(GLOB shader_source_editor_cpp_header_files *.hpp)
file(GLOB shader_source_editor_cpp_files *.cpp)
file(GLOB shader_source_editor_c_header_files *.h)
file(GLOB shader_source_editor_c_files *.c)

file(COPY main.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(shader_source_editor WIN32
	${shader_source_editor_cpp_header_files}
	${shader_source_editor_cpp_files}
	${shader_source_editor_c_header_files}
	${shader_source_editor_c_files})

target_link_libraries(shader_source_editor elengine)
target_link_libraries(shader_source_editor ${GTK_LIBRARIES})

