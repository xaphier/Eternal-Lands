project(elengine)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(angelscript/include)

if ((${GPA_FOUND} MATCHES "TRUE") AND (${BUILD_GPA} MATCHES "ON"))
include_directories(${GPA_INCLUDE_DIR})
endif ((${GPA_FOUND} MATCHES "TRUE") AND (${BUILD_GPA} MATCHES "ON"))

# project options
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 9)
set(${PROJECT_NAME}_PATCH_LEVEL 3)

math(EXPR ${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION} * 10000 + ${${PROJECT_NAME}_MINOR_VERSION} * 100 + ${${PROJECT_NAME}_PATCH_LEVEL}")
set(${PROJECT_NAME}_VERSION_STRING "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}")

file(GLOB engine_header_files *.hpp)
file(GLOB engine_source_files *.cpp)

#codec dir
file(GLOB codec_header_files codec/*.hpp)
file(GLOB codec_source_files codec/*.cpp)
list(APPEND engine_source_files ${codec_source_files})
list(APPEND engine_header_files ${codec_header_files})

#loader dir
file(GLOB loader_header_files loader/*.hpp)
file(GLOB loader_source_files loader/*.cpp)
list(APPEND engine_source_files ${loader_source_files})
list(APPEND engine_header_files ${loader_header_files})

#mesh dir
file(GLOB mesh_header_files mesh/*.hpp)
file(GLOB mesh_source_files mesh/*.cpp)
list(APPEND engine_source_files ${mesh_source_files})
list(APPEND engine_header_files ${mesh_header_files})

#shader dir
file(GLOB shader_header_files shader/*.hpp)
file(GLOB shader_source_files shader/*.cpp)
list(APPEND engine_source_files ${shader_source_files})
list(APPEND engine_header_files ${shader_header_files})

#framebuffer dir
file(GLOB framebuffer_header_files framebuffer/*.hpp)
file(GLOB framebuffer_source_files framebuffer/*.cpp)
list(APPEND engine_source_files ${framebuffer_source_files})
list(APPEND engine_header_files ${framebuffer_header_files})

#gui dir
file(GLOB gui_header_files gui/*.hpp)
file(GLOB gui_source_files gui/*.cpp)
list(APPEND engine_source_files ${gui_source_files})
list(APPEND engine_header_files ${gui_header_files})

##font dir
#file(GLOB font_header_files font/*.hpp)
#file(GLOB font_source_files font/*.cpp)
#list(APPEND engine_source_files ${font_source_files})
#list(APPEND engine_header_files ${font_header_files})

#terrain dir
file(GLOB terrain_header_files terrain/*.hpp)
file(GLOB terrain_source_files terrain/*.cpp)
list(APPEND engine_source_files ${terrain_source_files})
list(APPEND engine_header_files ${terrain_header_files})

#effect dir
file(GLOB effect_header_files effect/*.hpp)
file(GLOB effect_source_files effect/*.cpp)
list(APPEND engine_source_files ${effect_header_files})
list(APPEND engine_header_files ${effect_source_files})

#sha1 dir
file(GLOB sha1_header_files sha1/*.h)
file(GLOB sha1_source_files sha1/*.cpp)
list(APPEND engine_source_files ${sha1_source_files})
list(APPEND engine_header_files ${sha1_header_files})

#script dir
file(GLOB script_header_files script/*.h)
file(GLOB script_source_files script/*.cpp)
list(APPEND engine_source_files ${script_header_files})
list(APPEND engine_header_files ${script_source_files})

#thread dir
file(GLOB thread_header_files thread/*.h)
file(GLOB thread_source_files thread/*.cpp)
list(APPEND engine_source_files ${thread_header_files})
list(APPEND engine_header_files ${thread_source_files})

#hardwarebuffer dir
file(GLOB hardwarebuffer_header_files hardwarebuffer/*.hpp)
file(GLOB hardwarebuffer_source_files hardwarebuffer/*.cpp)
list(APPEND engine_source_files ${hardwarebuffer_header_files})
list(APPEND engine_header_files ${hardwarebuffer_source_files})

list(APPEND engine_source_files simd/simd.cpp)
list(APPEND engine_header_files simd/simd.hpp)
set_property(SOURCE simd/simd.cpp APPEND PROPERTY COMPILE_FLAGS ${SSE2_FLAGS})

if (ICONV_SECOND_ARGUMENT_IS_CONST)
	add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
endif (ICONV_SECOND_ARGUMENT_IS_CONST)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/buildinformations.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/buildinformations.cpp" @ONLY)

list(APPEND engine_source_files "${CMAKE_CURRENT_BINARY_DIR}/buildinformations.cpp")

add_subdirectory(lz4)
add_subdirectory(zip)
add_subdirectory(angelscript)
add_subdirectory(glsl_optimizer)

add_library(elengine ${engine_source_files} ${engine_header_files})

target_link_libraries(elengine ${GLEW_LIBRARY})
target_link_libraries(elengine ${OPENGL_LIBRARIES})
target_link_libraries(elengine ${CAL3D_LIBRARIES})
target_link_libraries(elengine ${JPEG_LIBRARIES})
target_link_libraries(elengine ${PNG_LIBRARIES})
#target_link_libraries(elengine ${Boost_LIBRARIES})
target_link_libraries(elengine ${ICONV_LIBRARIES})
target_link_libraries(elengine ${LIBXML2_LIBRARIES})
target_link_libraries(elengine ${SDL_LIBRARIES})
target_link_libraries(elengine ${SDL_LIBRARY})
target_link_libraries(elengine ${FREETYPE_LIBRARY})
target_link_libraries(elengine glsl_optimizer)
target_link_libraries(elengine angelscript)
target_link_libraries(elengine xz)
target_link_libraries(elengine zip)
target_link_libraries(elengine lz4)
target_link_libraries(elengine ${ZLIB_LIBRARIES})

if (OPENMP_FOUND)
	set_target_properties(elengine PROPERTIES COMPILE_FLAGS ${OpenMP_C_FLAGS})
	set_target_properties(elengine PROPERTIES LINKER_FLAGS ${OpenMP_C_FLAGS})
endif (OPENMP_FOUND)

if ((${GPA_FOUND} MATCHES "TRUE") AND (${BUILD_GPA} MATCHES "ON"))
target_link_libraries(elengine ${GPA_LIBRARY})
endif ((${GPA_FOUND} MATCHES "TRUE") AND (${BUILD_GPA} MATCHES "ON"))

if (PCHSupport_FOUND AND BUILD_PCH)
	ADD_PRECOMPILED_HEADER(elengine prerequisites.hpp)
endif (PCHSupport_FOUND AND BUILD_PCH)

set_target_properties(elengine PROPERTIES VERSION ${${PROJECT_NAME}_VERSION_STRING})

