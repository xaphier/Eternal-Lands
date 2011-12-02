project(elluac++)

# project options
set(${PROJECT_NAME}_MAJOR_VERSION 5)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_PATCH_LEVEL 4)

math(EXPR ${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION} * 10000 + ${${PROJECT_NAME}_MINOR_VERSION} * 100 + ${${PROJECT_NAME}_PATCH_LEVEL}")
set(${PROJECT_NAME}_VERSION_STRING "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}")


file(GLOB lua_cpp_header_files *.h)
file(GLOB lua_cpp_files *.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_definitions(-DLUA_USE_POSIX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_definitions(-DLUA_USE_POSIX)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_library(elluac++ ${lua_cpp_files} ${lua_cpp_header_files})

set_target_properties(elluac++ PROPERTIES VERSION ${${PROJECT_NAME}_VERSION_STRING})
