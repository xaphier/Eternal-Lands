project(angelscript)

# project options
set(${PROJECT_NAME}_MAJOR_VERSION 2)
set(${PROJECT_NAME}_MINOR_VERSION 23)
set(${PROJECT_NAME}_PATCH_LEVEL 0)

math(EXPR ${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION} * 10000 + ${${PROJECT_NAME}_MINOR_VERSION} * 100 + ${${PROJECT_NAME}_PATCH_LEVEL}")
set(${PROJECT_NAME}_VERSION_STRING "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}")

file(GLOB_RECURSE angelscript_header_files *.h)
file(GLOB_RECURSE angelscript_source_files *.cpp)

add_library(angelscript ${angelscript_source_files} ${angelscript_header_files})

set_target_properties(angelscript PROPERTIES VERSION ${${PROJECT_NAME}_VERSION_STRING})
