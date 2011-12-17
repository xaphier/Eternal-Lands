project(elengine)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# project options
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 9)
set(${PROJECT_NAME}_PATCH_LEVEL 0)

math(EXPR ${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION} * 10000 + ${${PROJECT_NAME}_MINOR_VERSION} * 100 + ${${PROJECT_NAME}_PATCH_LEVEL}")
set(${PROJECT_NAME}_VERSION_STRING "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}")

file(GLOB engine_header_files *.hpp)
file(GLOB engine_cpp_files *.cpp)

#codec dir
file(GLOB codec_header_files codec/*.hpp)
file(GLOB codec_cpp_files codec/*.cpp)
list(APPEND engine_cpp_files ${codec_cpp_files})
list(APPEND engine_header_files ${codec_header_files})

#loader dir
file(GLOB loader_header_files loader/*.hpp)
file(GLOB loader_cpp_files loader/*.cpp)
list(APPEND engine_cpp_files ${loader_cpp_files})
list(APPEND engine_header_files ${loader_header_files})

#mesh dir
file(GLOB mesh_header_files mesh/*.hpp)
file(GLOB mesh_cpp_files mesh/*.cpp)
list(APPEND engine_cpp_files ${mesh_cpp_files})
list(APPEND engine_header_files ${mesh_header_files})

#shader dir
file(GLOB shader_header_files shader/*.hpp)
file(GLOB shader_cpp_files shader/*.cpp)
list(APPEND engine_cpp_files ${shader_cpp_files})
list(APPEND engine_header_files ${shader_header_files})

#framebuffer dir
file(GLOB framebuffer_header_files framebuffer/*.hpp)
file(GLOB framebuffer_cpp_files framebuffer/*.cpp)
list(APPEND engine_cpp_files ${framebuffer_header_files})
list(APPEND engine_header_files ${framebuffer_cpp_files})

#gui dir
file(GLOB gui_header_files gui/*.hpp)
file(GLOB gui_cpp_files gui/*.cpp)
list(APPEND engine_cpp_files ${gui_header_files})
list(APPEND engine_header_files ${gui_cpp_files})

#font dir
file(GLOB font_header_files font/*.hpp)
file(GLOB font_cpp_files font/*.cpp)
list(APPEND engine_cpp_files ${font_header_files})
list(APPEND engine_header_files ${font_cpp_files})

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/buildinformations.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/buildinformations.cpp" @ONLY)

list(APPEND engine_cpp_files "${CMAKE_CURRENT_BINARY_DIR}/buildinformations.cpp")

add_subdirectory(lua)
add_subdirectory(glsl_optimizer)

add_library(elengine ${engine_cpp_files} ${engine_header_files})

target_link_libraries(elengine ${CAL3D_LIBRARIES})
target_link_libraries(elengine ${JPEG_LIBRARIES})
target_link_libraries(elengine ${PNG_LIBRARIES})
target_link_libraries(elengine ${ZLIB_LIBRARIES})
target_link_libraries(elengine ${Boost_LIBRARIES})
target_link_libraries(elengine ${OPENGL_LIBRARIES})
target_link_libraries(elengine ${ICONV_LIBRARIES})
target_link_libraries(elengine ${LIBXML2_LIBRARIES})
target_link_libraries(elengine ${SDL_LIBRARIES})
target_link_libraries(elengine ${GLEW_LIBRARY})
target_link_libraries(elengine glsl_optimizer)
target_link_libraries(elengine elluac++)
target_link_libraries(elengine xz)

IF (PCHSupport_FOUND AND BUILD_PCH)
	ADD_PRECOMPILED_HEADER(elengine prerequisites.hpp)
ENDIF (PCHSupport_FOUND AND BUILD_PCH)

set_target_properties(elengine PROPERTIES VERSION ${${PROJECT_NAME}_VERSION_STRING})

