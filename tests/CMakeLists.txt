ENABLE_TESTING()

include(FindOpenAL)

link_directories(${TESTS_BINARY_DIR})

find_package(Boost 1.40.0 COMPONENTS unit_test_framework REQUIRED)

add_definitions(-DBOOST_TEST_DYN_LINK)

file(GLOB default default/*.cpp)

foreach(FILE_NAME ${default})
	file(RELATIVE_PATH PROG_NAME ${CMAKE_CURRENT_SOURCE_DIR}/default ${FILE_NAME})

	string(REPLACE ".cpp" "" PROG_NAME ${PROG_NAME})

	add_executable(${PROG_NAME} ${FILE_NAME})

	target_link_libraries(${PROG_NAME} elengine)
	target_link_libraries(${PROG_NAME} elluac++)
	target_link_libraries(${PROG_NAME} ${CAL3D_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${JPEG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${PNG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${ZLIB_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${Boost_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${ICONV_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${LIBXML2_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CMAKE_THREAD_LIBS_INIT})

	add_test(${PROG_NAME} ${EXECUTABLE_OUTPUT_PATH}/${PROG_NAME})
endforeach(FILE_NAME)

file(GLOB simple simple/*.cpp)

foreach(FILE_NAME ${simple})
	file(RELATIVE_PATH PROG_NAME ${CMAKE_CURRENT_SOURCE_DIR}/simple ${FILE_NAME})

	string(REPLACE ".cpp" "" PROG_NAME ${PROG_NAME})

	add_executable(${PROG_NAME} ${FILE_NAME})

	target_link_libraries(${PROG_NAME} elengine)
	target_link_libraries(${PROG_NAME} elluac++)
	target_link_libraries(${PROG_NAME} ${CAL3D_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${JPEG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${PNG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${ZLIB_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${Boost_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${ICONV_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${LIBXML2_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CMAKE_THREAD_LIBS_INIT})

	add_test(${PROG_NAME} ${EXECUTABLE_OUTPUT_PATH}/${PROG_NAME})
endforeach(FILE_NAME)

file(GLOB enum enum/*.cpp)

foreach(FILE_NAME ${enum})
	file(RELATIVE_PATH PROG_NAME ${CMAKE_CURRENT_SOURCE_DIR}/enum ${FILE_NAME})

	string(REPLACE ".cpp" "" PROG_NAME ${PROG_NAME})

	add_executable(${PROG_NAME} ${FILE_NAME})

	target_link_libraries(${PROG_NAME} elengine)
	target_link_libraries(${PROG_NAME} elluac++)
	target_link_libraries(${PROG_NAME} ${CAL3D_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${JPEG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${PNG_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${ZLIB_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${Boost_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${ICONV_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${LIBXML2_LIBRARIES})
	target_link_libraries(${PROG_NAME} ${CMAKE_THREAD_LIBS_INIT})

	add_test(${PROG_NAME} ${EXECUTABLE_OUTPUT_PATH}/${PROG_NAME})
endforeach(FILE_NAME)

foreach(NAME check_glm)
	file(GLOB ${NAME}_header_files ${NAME}/*.hpp)
	file(GLOB ${NAME}_cpp_files ${NAME}/*.cpp)

	add_executable(${NAME} ${${NAME}_cpp_files} ${${NAME}_header_files})

	target_link_libraries(${NAME} elengine)
	target_link_libraries(${NAME} elluac++)
	target_link_libraries(${NAME} ${CAL3D_LIBRARIES})
	target_link_libraries(${NAME} ${JPEG_LIBRARIES})
	target_link_libraries(${NAME} ${PNG_LIBRARIES})
	target_link_libraries(${NAME} ${ZLIB_LIBRARIES})
	target_link_libraries(${NAME} ${Boost_LIBRARIES})
	target_link_libraries(${NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${NAME} ${ICONV_LIBRARIES})
	target_link_libraries(${NAME} ${LIBXML2_LIBRARIES})
	target_link_libraries(${NAME} ${CMAKE_THREAD_LIBS_INIT})

	add_test(${NAME} ${EXECUTABLE_OUTPUT_PATH}/${NAME})
endforeach(NAME)
