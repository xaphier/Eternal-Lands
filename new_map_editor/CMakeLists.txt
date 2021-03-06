cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(map_editor)

SET(QT_USE_QTOPENGL TRUE)
FIND_PACKAGE(Qt4 REQUIRED)

include(${QT_USE_FILE}) 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB map_editor_header_files *.hpp)
file(GLOB map_editor_source_files *.cpp)
file(GLOB map_editor_ui_files *.ui)
file(GLOB map_editor_qrc_files icons/*.qrc)

#nodes dir
file(GLOB nodes_header_files nodes/*.hpp)
file(GLOB nodes_source_files nodes/*.cpp)
file(GLOB nodes_ui_files nodes/*.ui)
list(APPEND map_editor_source_files ${nodes_source_files})
list(APPEND map_editor_header_files ${nodes_header_files})
list(APPEND map_editor_ui_files ${nodes_ui_files})

QT4_WRAP_CPP(map_editor_moc_files ${map_editor_header_files} OPTIONS -DBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
QT4_WRAP_UI(map_editor_form_files ${map_editor_ui_files})
QT4_ADD_RESOURCES(map_editor_resource_files ${map_editor_qrc_files})

#editor dir
file(GLOB editor_header_files editor/*.hpp)
file(GLOB editor_source_files editor/*.cpp)
list(APPEND map_editor_source_files ${editor_source_files})
list(APPEND map_editor_header_files ${editor_header_files})

#editor/undo dir
file(GLOB editor_undo_header_files editor/undo/*.hpp)
file(GLOB editor_undo_source_files editor/undo/*.cpp)
list(APPEND map_editor_source_files ${editor_undo_source_files})
list(APPEND map_editor_header_files ${editor_undo_header_files})

add_executable(map_editor WIN32 ${map_editor_header_files}
	${map_editor_source_files}
	${map_editor_moc_files}
	${map_editor_form_files}
	${map_editor_resource_files})

target_link_libraries(map_editor ${GLEW_LIBRARY})
target_link_libraries(map_editor ${OPENGL_LIBRARIES})
target_link_libraries(map_editor ${CAL3D_LIBRARIES})
target_link_libraries(map_editor ${JPEG_LIBRARIES})
target_link_libraries(map_editor ${PNG_LIBRARIES})
#target_link_libraries(map_editor ${Boost_LIBRARIES})
target_link_libraries(map_editor ${ICONV_LIBRARIES})
target_link_libraries(map_editor ${LIBXML2_LIBRARIES})
target_link_libraries(map_editor ${SDL_LIBRARIES})
target_link_libraries(map_editor ${SDL_LIBRARY})
target_link_libraries(map_editor ${FREETYPE_LIBRARY})
target_link_libraries(map_editor elengine)
target_link_libraries(map_editor ${QT_LIBRARIES})
target_link_libraries(map_editor ${ZLIB_LIBRARIES})
if ((${OPENMP_FOUND} MATCHES "TRUE") AND (${BUILD_OPENMP} MATCHES "ON"))
set_target_properties(map_editor PROPERTIES COMPILE_FLAGS ${OpenMP_CXX_FLAGS})
target_link_libraries(map_editor ${OpenMP_LIB})
endif ((${OPENMP_FOUND} MATCHES "TRUE") AND (${BUILD_OPENMP} MATCHES "ON"))

if (WIN32)
	target_link_libraries(map_editor bfd)
	target_link_libraries(map_editor intl)
	target_link_libraries(map_editor iberty)
	target_link_libraries(map_editor imagehlp)
endif (WIN32)
