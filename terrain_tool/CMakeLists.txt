cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(terrain_tool)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB terrain_tool_header_files *.hpp)
file(GLOB terrain_tool_source_files *.cpp)

add_executable(terrain_tool WIN32 ${terrain_tool_header_files}
	${terrain_tool_source_files})

target_link_libraries(terrain_tool ${GLEW_LIBRARY})
target_link_libraries(terrain_tool ${OPENGL_LIBRARIES})
target_link_libraries(terrain_tool ${CAL3D_LIBRARIES})
target_link_libraries(terrain_tool ${JPEG_LIBRARIES})
target_link_libraries(terrain_tool ${PNG_LIBRARIES})
#target_link_libraries(terrain_tool ${Boost_LIBRARIES})
target_link_libraries(terrain_tool ${ICONV_LIBRARIES})
target_link_libraries(terrain_tool ${LIBXML2_LIBRARIES})
target_link_libraries(terrain_tool ${SDL_LIBRARIES})
target_link_libraries(terrain_tool ${SDL_LIBRARY})
target_link_libraries(terrain_tool ${FREETYPE_LIBRARY})
target_link_libraries(terrain_tool elengine)
target_link_libraries(terrain_tool ${ZLIB_LIBRARIES})

if (WIN32)
	target_link_libraries(el_client bfd)
	target_link_libraries(el_client iberty)
	target_link_libraries(el_client imagehlp)
endif (WIN32)
