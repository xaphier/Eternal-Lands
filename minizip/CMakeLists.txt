project(minizip)

# project options
set(${PROJECT_NAME}_MAJOR_VERSION 1)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_PATCH_LEVEL 0)

math(EXPR ${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION} * 10000 + ${${PROJECT_NAME}_MINOR_VERSION} * 100 + ${${PROJECT_NAME}_PATCH_LEVEL}")
set(${PROJECT_NAME}_VERSION_STRING "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}")


file(GLOB minizip_header_files *.h)
file(GLOB minizip_source_files *.c)

add_library(minizip ${minizip_source_files} ${minizip_header_files})

set_target_properties(minizip PROPERTIES VERSION ${${PROJECT_NAME}_VERSION_STRING})
