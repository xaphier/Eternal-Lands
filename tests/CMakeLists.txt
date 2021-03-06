ENABLE_TESTING()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${TESTS_BINARY_DIR})

find_package(Boost 1.40.0 COMPONENTS unit_test_framework REQUIRED)

include(CheckCXXSourceCompiles)

set(CMAKE_REQUIRED_LIBRARIES ${Boost_LIBRARIES})

check_cxx_source_compiles("
	#define BOOST_TEST_MODULE tes
	#include <boost/test/unit_test.hpp>
	"
	BOOST_STATIC_TEST_LIB)

if (NOT BOOST_STATIC_TEST_LIB)
	add_definitions(-DBOOST_TEST_DYN_LINK)
endif (NOT BOOST_STATIC_TEST_LIB)

file(COPY ../data/materials.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB shader_sources ../data/shaders/sources/*.xml)
file(GLOB default default/*.cpp)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/shader_sources.txt "")

foreach (shader_source ${shader_sources})
	file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/shader_sources.txt
		${shader_source})
	file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/shader_sources.txt "\n")
endforeach (shader_source)

foreach (FILE_NAME ${default})
	file(RELATIVE_PATH PROG_NAME ${CMAKE_CURRENT_SOURCE_DIR}/default
		${FILE_NAME})

	string(REPLACE ".cpp" "" PROG_NAME ${PROG_NAME})

	add_executable(${PROG_NAME} ${FILE_NAME})

	target_link_libraries(${PROG_NAME} ${GLEW_LIBRARY})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CAL3D_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${JPEG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${PNG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${Boost_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${ICONV_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${LIBXML2_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CMAKE_THREAD_LIBS_INIT})
	target_link_libraries(${PROG_NAME} ${ZLIB_LIBRARIES})
	target_link_libraries(${PROG_NAME} elengine)
	if ((${OPENMP_FOUND} MATCHES "TRUE") AND (${BUILD_OPENMP} MATCHES "ON"))
	target_link_libraries(${PROG_NAME} ${OpenMP_LIB})
	endif ((${OPENMP_FOUND} MATCHES "TRUE") AND (${BUILD_OPENMP} MATCHES "ON"))

	add_test(${PROG_NAME} ${EXECUTABLE_OUTPUT_PATH}/${PROG_NAME})
endforeach (FILE_NAME)

file(GLOB simple simple/*.cpp)

foreach (FILE_NAME ${simple})
	file(RELATIVE_PATH PROG_NAME ${CMAKE_CURRENT_SOURCE_DIR}/simple
		${FILE_NAME})

	string(REPLACE ".cpp" "" PROG_NAME ${PROG_NAME})

	add_executable(${PROG_NAME} ${FILE_NAME})

	target_link_libraries(${PROG_NAME} ${GLEW_LIBRARY})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CAL3D_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${JPEG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${PNG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${Boost_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${ICONV_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${LIBXML2_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CMAKE_THREAD_LIBS_INIT})
	target_link_libraries(${PROG_NAME} ${ZLIB_LIBRARIES})
	target_link_libraries(${PROG_NAME} elengine)
	if ((${OPENMP_FOUND} MATCHES "TRUE") AND (${BUILD_OPENMP} MATCHES "ON"))
	target_link_libraries(${PROG_NAME} ${OpenMP_LIB})
	endif ((${OPENMP_FOUND} MATCHES "TRUE") AND (${BUILD_OPENMP} MATCHES "ON"))

	add_test(${PROG_NAME} ${EXECUTABLE_OUTPUT_PATH}/${PROG_NAME})
endforeach (FILE_NAME)

file(GLOB enum enum/*.cpp)

foreach (FILE_NAME ${enum})
	file(RELATIVE_PATH PROG_NAME ${CMAKE_CURRENT_SOURCE_DIR}/enum
		${FILE_NAME})

	string(REPLACE ".cpp" "" PROG_NAME ${PROG_NAME})

	add_executable(${PROG_NAME} ${FILE_NAME})

	target_link_libraries(${PROG_NAME} ${GLEW_LIBRARY})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CAL3D_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${JPEG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${PNG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${Boost_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${ICONV_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${LIBXML2_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CMAKE_THREAD_LIBS_INIT})
	target_link_libraries(${PROG_NAME} ${ZLIB_LIBRARIES})
	target_link_libraries(${PROG_NAME} elengine)
	if ((${OPENMP_FOUND} MATCHES "TRUE") AND (${BUILD_OPENMP} MATCHES "ON"))
	target_link_libraries(${PROG_NAME} ${OpenMP_LIB})
	endif ((${OPENMP_FOUND} MATCHES "TRUE") AND (${BUILD_OPENMP} MATCHES "ON"))

	add_test(${PROG_NAME} ${EXECUTABLE_OUTPUT_PATH}/${PROG_NAME})
endforeach (FILE_NAME)

file(GLOB io io/*.cpp)

foreach (FILE_NAME ${io})
	file(RELATIVE_PATH PROG_NAME ${CMAKE_CURRENT_SOURCE_DIR}/io
		${FILE_NAME})

	string(REPLACE ".cpp" "" PROG_NAME ${PROG_NAME})

	add_executable(${PROG_NAME} ${FILE_NAME})

	target_link_libraries(${PROG_NAME} ${GLEW_LIBRARY})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CAL3D_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${JPEG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${PNG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${Boost_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${ICONV_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${LIBXML2_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CMAKE_THREAD_LIBS_INIT})
	target_link_libraries(${PROG_NAME} ${ZLIB_LIBRARIES})
	target_link_libraries(${PROG_NAME} elengine)
	if ((${OPENMP_FOUND} MATCHES "TRUE") AND (${BUILD_OPENMP} MATCHES "ON"))
	target_link_libraries(${PROG_NAME} ${OpenMP_LIB})
	endif ((${OPENMP_FOUND} MATCHES "TRUE") AND (${BUILD_OPENMP} MATCHES "ON"))

	add_test(${PROG_NAME} ${EXECUTABLE_OUTPUT_PATH}/${PROG_NAME})
endforeach (FILE_NAME)
