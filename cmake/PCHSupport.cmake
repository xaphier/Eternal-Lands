# - Try to find precompiled headers support for GCC 3.4 and 4.x
# Once done this will define:
#
# Variable:
#   PCHSupport_FOUND
#
# Macro:
#   ADD_PRECOMPILED_HEADER

IF (CMAKE_COMPILER_IS_GNUCXX)
	EXEC_PROGRAM(
		${CMAKE_CXX_COMPILER} 
		ARGS                    --version 
		OUTPUT_VARIABLE _compiler_output)
	STRING(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1" 
		gcc_compiler_version ${_compiler_output})
	#MESSAGE("GCC Version: ${gcc_compiler_version}")
	IF (gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
		SET(PCHSupport_FOUND TRUE)
	ELSE (gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
		IF (gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
			SET(PCHSupport_FOUND TRUE)
		ENDIF (gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
	ENDIF (gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
	IF (PCHSupport_FOUND)
		ADD_DEFINITIONS( -DUSE_PCH=1 )
		SET(PCHSupport_COMPILER "GCC")
	ELSE (PCHSupport_FOUND)
		ADD_DEFINITIONS( -DUSE_PCH=0 )
	ENDIF (PCHSupport_FOUND)
ELSE (CMAKE_COMPILER_IS_GNUCXX)
	IF (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		SET(PCHSupport_COMPILER "Clang")
		SET(PCHSupport_FOUND TRUE)
		ADD_DEFINITIONS( -DUSE_PCH=1 )
	ENDIF (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

# ADD_PRECOMPILED_HEADER_GCC( targetName HEADER _input )
MACRO(ADD_PRECOMPILED_HEADER_GCC _targetName _input )
	GET_FILENAME_COMPONENT(_name ${_input} NAME)
	SET(_source "${CMAKE_CURRENT_SOURCE_DIR}/${_input}")
	SET(_outdir "${CMAKE_CURRENT_BINARY_DIR}/${_name}.gch")
	MAKE_DIRECTORY(${_outdir})
	SET(_output "${_outdir}/${CMAKE_BUILD_TYPE}.c++")
	STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
	SET(_compiler_FLAGS ${${_flags_var_name}})

	GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
	FOREACH(item ${_directory_flags})
		LIST(APPEND _compiler_FLAGS "-I${item}")
	ENDFOREACH(item)
	FOREACH(item ${CMAKE_CXX_FLAGS})
		LIST(APPEND _compiler_FLAGS "${item}")
	ENDFOREACH(item)

	GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
	LIST(APPEND _compiler_FLAGS ${_directory_flags})

	SEPARATE_ARGUMENTS(_compiler_FLAGS)
	#MESSAGE("_compiler_FLAGS: ${_compiler_FLAGS}")
	#message("${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header -o ${_output} ${_source}")
	ADD_CUSTOM_COMMAND(
		OUTPUT ${_output}
		COMMAND ${CMAKE_CXX_COMPILER}
		${_compiler_FLAGS}
		-x c++-header
		-o ${_output} ${_source}
		DEPENDS ${_source} )
	ADD_CUSTOM_TARGET(${_targetName}_gch DEPENDS ${_output})
	ADD_DEPENDENCIES(${_targetName} ${_targetName}_gch)
	#SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-include ${_name} -Winvalid-pch -H")
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${_name} -Winvalid-pch")
	SET_TARGET_PROPERTIES(${_targetName} PROPERTIES
		COMPILE_FLAGS "-include ${_name} -Winvalid-pch"
		)
ENDMACRO(ADD_PRECOMPILED_HEADER_GCC)

# ADD_PRECOMPILED_HEADER_CLANG( targetName HEADER _input )
MACRO(ADD_PRECOMPILED_HEADER_CLANG _targetName _input )
	GET_FILENAME_COMPONENT(_name ${_input} NAME)
	SET(_source "${CMAKE_CURRENT_SOURCE_DIR}/${_input}")
	SET(_outdir "${CMAKE_CURRENT_BINARY_DIR}/${_name}.gch")
	MAKE_DIRECTORY(${_outdir})
	SET(_output "${_outdir}/${CMAKE_BUILD_TYPE}.c++")
	STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
	SET(_compiler_FLAGS ${${_flags_var_name}})

	GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
	FOREACH(item ${_directory_flags})
		LIST(APPEND _compiler_FLAGS "-I${item}")
	ENDFOREACH(item)
	FOREACH(item ${CMAKE_CXX_FLAGS})
		LIST(APPEND _compiler_FLAGS "${item}")
	ENDFOREACH(item)

	GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
	LIST(APPEND _compiler_FLAGS ${_directory_flags})

	SEPARATE_ARGUMENTS(_compiler_FLAGS)
	#MESSAGE("_compiler_FLAGS: ${_compiler_FLAGS}")
	#message("${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header -o ${_output} ${_source}")
	ADD_CUSTOM_COMMAND(
		OUTPUT ${_output}
		COMMAND ${CMAKE_CXX_COMPILER}
		${_compiler_FLAGS}
		-x c++-header
		-o ${_output} ${_source}
		DEPENDS ${_source} )
	ADD_CUSTOM_TARGET(${_targetName}_gch DEPENDS ${_output})
	ADD_DEPENDENCIES(${_targetName} ${_targetName}_gch)
	#SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-include ${_name} -Winvalid-pch -H")
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${_name} -Winvalid-pch")
	SET_TARGET_PROPERTIES(${_targetName} PROPERTIES
		COMPILE_FLAGS "-include-pch ${_output} -Winvalid-pch"
		)
ENDMACRO(ADD_PRECOMPILED_HEADER_CLANG)

# ADD_PRECOMPILED_HEADER( targetName HEADERS _inputs )
MACRO(ADD_PRECOMPILED_HEADER _targetName _inputs)
	IF (PCHSupport_COMPILER MATCHES "Clang")
		ADD_PRECOMPILED_HEADER_CLANG(${_targetName} ${_inputs})
	ELSEIF (PCHSupport_COMPILER MATCHES "GCC")
		ADD_PRECOMPILED_HEADER_GCC(${_targetName} ${_inputs})
	ENDIF (PCHSupport_COMPILER MATCHES "Clang")
ENDMACRO(ADD_PRECOMPILED_HEADER)

