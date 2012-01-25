cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(material_editor)

SET(QT_USE_QTOPENGL TRUE)
FIND_PACKAGE(Qt4 REQUIRED)

include(${QT_USE_FILE}) 
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB material_editor_header_files *.hpp)
file(GLOB material_editor_source_files *.cpp)
file(GLOB material_editor_ui_files *.ui)
file(GLOB material_editor_qrc_files *.qrc)

QT4_WRAP_CPP(material_editor_moc_files ${material_editor_header_files})
QT4_WRAP_UI(material_editor_form_files ${material_editor_ui_files})
QT4_ADD_RESOURCES(material_editor_resource_files ${material_editor_qrc_files})

add_executable(material_editor WIN32 ${material_editor_header_files}
	${material_editor_source_files}
	${material_editor_moc_files}
	${material_editor_form_files}
	${material_editor_resource_files})

target_link_libraries(material_editor elengine)
target_link_libraries(material_editor ${QT_LIBRARIES})

