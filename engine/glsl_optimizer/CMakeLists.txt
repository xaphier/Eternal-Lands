cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(glsl_optimizer)

#include_directories(${glsl_optimizer_SOURCE_DIR})
include_directories(${glsl_optimizer_SOURCE_DIR}/glsl)
include_directories(${glsl_optimizer_SOURCE_DIR}/mesa)
include_directories(${glsl_optimizer_SOURCE_DIR}/mapi)

file(GLOB glsl_optimizer_cpp_files glsl/*.cpp)
file(GLOB glsl_optimizer_header_files glsl/*.h)
file(GLOB glsl_optimizer_c_files glsl/*.c)

list(APPEND glsl_optimizer_c_files glsl/glcpp/glcpp-lex.c)
list(APPEND glsl_optimizer_c_files glsl/glcpp/glcpp-parse.c)
list(APPEND glsl_optimizer_c_files glsl/glcpp/pp.c)

list(APPEND glsl_optimizer_c_files mesa/program/hash_table.c)
list(APPEND glsl_optimizer_c_files mesa/program/symbol_table.c)

add_definitions(-O)

add_library(glsl_optimizer ${glsl_optimizer_header_files}
	${glsl_optimizer_cpp_files} ${glsl_optimizer_c_files})

