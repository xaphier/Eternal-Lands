project(xz)

file(GLOB xz_c_header_files *.h)
file(GLOB xz_c_files *.c)

add_definitions(-D_7ZIP_ST)

add_library(xz ${xz_c_files} ${xz_c_header_files})

