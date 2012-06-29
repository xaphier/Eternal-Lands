project(lz4)

# project options
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 68)
set(${PROJECT_NAME}_PATCH_LEVEL 0)

math(EXPR ${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION} * 10000 + ${${PROJECT_NAME}_MINOR_VERSION} * 100 + ${${PROJECT_NAME}_PATCH_LEVEL}")
set(${PROJECT_NAME}_VERSION_STRING "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}")

file(GLOB lz4_header_files *.h)
file(GLOB lz4_source_files *.c)

add_library(lz4 ${lz4_source_files} ${lz4_header_files})

set_target_properties(lz4 PROPERTIES VERSION ${${PROJECT_NAME}_VERSION_STRING})
