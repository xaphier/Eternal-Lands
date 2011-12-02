project(eye_candy)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB eye_candy_header_files *.hpp)
file(GLOB eye_candy_cpp_files *.cpp)

add_library(eye_candy ${eye_candy_cpp_files} ${eye_candy_header_files})

IF (PCHSupport_FOUND AND BUILD_PCH)
	ADD_PRECOMPILED_HEADER(eye_candy eye_candy.h)
ENDIF (PCHSupport_FOUND AND BUILD_PCH)

