# - Find glm
# Find the native glm includes and library
#
#  GLM_INCLUDE_DIR - where to find glm.h, etc.
#  GLM_FOUND       - True if glm found.

IF (GLM_INCLUDE_DIR)
  # Already in cache, be silent
  SET(GLM_FIND_QUIETLY TRUE)
ENDIF (GLM_INCLUDE_DIR)

FIND_PATH(GLM_INCLUDE_DIR glm/glm.hpp)

IF(GLM_INCLUDE_DIR) 
   SET(GLM_FOUND TRUE) 
ENDIF(GLM_INCLUDE_DIR) 

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLM DEFAULT_MSG GLM_INCLUDE_DIR)
MARK_AS_ADVANCED( GLM_INCLUDE_DIR )
