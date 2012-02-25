cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(map_editor)

SET(QT_USE_QTOPENGL TRUE)
FIND_PACKAGE(Qt4 REQUIRED)

include(${QT_USE_FILE}) 
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB map_editor_header_files *.hpp)
file(GLOB map_editor_source_files *.cpp)
file(GLOB map_editor_ui_files *.ui)
file(GLOB map_editor_qrc_files *.qrc)

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

QT4_WRAP_CPP(material_editor_moc_files ${map_editor_header_files} OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
QT4_WRAP_UI(material_editor_form_files ${map_editor_ui_files})
QT4_ADD_RESOURCES(material_editor_resource_files ${map_editor_qrc_files})

add_executable(map_editor WIN32 ${map_editor_header_files}
	${map_editor_source_files}
	${map_editor_moc_files}
	${map_editor_form_files}
	${map_editor_resource_files})

target_link_libraries(map_editor elengine)
target_link_libraries(map_editor ${QT_LIBRARIES})
