cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(shader_source_editor)

SET(QT_USE_QTOPENGL TRUE)
FIND_PACKAGE(Qt4 REQUIRED)

include(${QT_USE_FILE}) 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB shader_source_editor_header_files *.hpp)
file(GLOB shader_source_editor_source_files *.cpp)
file(GLOB shader_source_editor_ui_files *.ui)
file(GLOB shader_source_editor_qrc_files icons/*.qrc)

QT4_WRAP_CPP(shader_source_editor_moc_files ${shader_source_editor_header_files} OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
QT4_WRAP_UI(shader_source_editor_form_files ${shader_source_editor_ui_files})
QT4_ADD_RESOURCES(shader_source_editor_resource_files ${shader_source_editor_qrc_files})

add_executable(shader_source_editor WIN32 ${shader_source_editor_header_files}
	${shader_source_editor_source_files}
	${shader_source_editor_moc_files}
	${shader_source_editor_form_files}
	${shader_source_editor_resource_files})

target_link_libraries(shader_source_editor ${GLEW_LIBRARY})
target_link_libraries(shader_source_editor ${OPENGL_LIBRARIES})
target_link_libraries(shader_source_editor ${CAL3D_LIBRARIES})
target_link_libraries(shader_source_editor ${JPEG_LIBRARIES})
target_link_libraries(shader_source_editor ${PNG_LIBRARIES})
#target_link_libraries(shader_source_editor ${Boost_LIBRARIES})
target_link_libraries(shader_source_editor ${ICONV_LIBRARIES})
target_link_libraries(shader_source_editor ${LIBXML2_LIBRARIES})
target_link_libraries(shader_source_editor ${SDL_LIBRARIES})
target_link_libraries(shader_source_editor ${SDL_LIBRARY})
target_link_libraries(shader_source_editor ${FREETYPE_LIBRARY})
target_link_libraries(shader_source_editor elengine)
target_link_libraries(shader_source_editor ${QT_LIBRARIES})
target_link_libraries(shader_source_editor ${ZLIB_LIBRARIES})

if (WIN32)
	target_link_libraries(shader_source_editor bfd)
	target_link_libraries(shader_source_editor iberty)
	target_link_libraries(shader_source_editor imagehlp)
endif (WIN32)
