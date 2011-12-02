/****************************************************************************
 *            glmutil.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_12de4276_67ad_4a11_a906_6f9d50df06e1
#define	UUID_12de4276_67ad_4a11_a906_6f9d50df06e1

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class XmlUtil.
 * This file contains the @c class XmlUtil.
 */
namespace eternal_lands
{

	OutStream& operator<<(OutStream &str, const glm::vec2 &value);
	OutStream& operator<<(OutStream &str, const glm::vec3 &value);
	OutStream& operator<<(OutStream &str, const glm::vec4 &value);
	OutStream& operator<<(OutStream &str, const glm::bvec2 &value);
	OutStream& operator<<(OutStream &str, const glm::bvec3 &value);
	OutStream& operator<<(OutStream &str, const glm::bvec4 &value);
	OutStream& operator<<(OutStream &str, const glm::ivec2 &value);
	OutStream& operator<<(OutStream &str, const glm::ivec3 &value);
	OutStream& operator<<(OutStream &str, const glm::ivec4 &value);
	OutStream& operator<<(OutStream &str, const glm::uvec2 &value);
	OutStream& operator<<(OutStream &str, const glm::uvec3 &value);
	OutStream& operator<<(OutStream &str, const glm::uvec4 &value);
	OutStream& operator<<(OutStream &str, const glm::mat2x2 &value);
	OutStream& operator<<(OutStream &str, const glm::mat2x3 &value);
	OutStream& operator<<(OutStream &str, const glm::mat2x4 &value);
	OutStream& operator<<(OutStream &str, const glm::mat3x2 &value);
	OutStream& operator<<(OutStream &str, const glm::mat3x3 &value);
	OutStream& operator<<(OutStream &str, const glm::mat3x4 &value);
	OutStream& operator<<(OutStream &str, const glm::mat4x2 &value);
	OutStream& operator<<(OutStream &str, const glm::mat4x3 &value);
	OutStream& operator<<(OutStream &str, const glm::mat4x4 &value);
	InStream& operator>>(InStream &str, glm::vec2 &value);
	InStream& operator>>(InStream &str, glm::vec3 &value);
	InStream& operator>>(InStream &str, glm::vec4 &value);
	InStream& operator>>(InStream &str, glm::bvec2 &value);
	InStream& operator>>(InStream &str, glm::bvec3 &value);
	InStream& operator>>(InStream &str, glm::bvec4 &value);
	InStream& operator>>(InStream &str, glm::ivec2 &value);
	InStream& operator>>(InStream &str, glm::ivec3 &value);
	InStream& operator>>(InStream &str, glm::ivec4 &value);
	InStream& operator>>(InStream &str, glm::uvec2 &value);
	InStream& operator>>(InStream &str, glm::uvec3 &value);
	InStream& operator>>(InStream &str, glm::uvec4 &value);
	InStream& operator>>(InStream &str, glm::mat2x2 &value);
	InStream& operator>>(InStream &str, glm::mat2x3 &value);
	InStream& operator>>(InStream &str, glm::mat2x4 &value);
	InStream& operator<<(InStream &str, glm::mat3x2 &value);
	InStream& operator>>(InStream &str, glm::mat3x3 &value);
	InStream& operator>>(InStream &str, glm::mat3x4 &value);
	InStream& operator>>(InStream &str, glm::mat4x2 &value);
	InStream& operator>>(InStream &str, glm::mat4x3 &value);
	InStream& operator>>(InStream &str, glm::mat4x4 &value);

}

#endif	/* UUID_12de4276_67ad_4a11_a906_6f9d50df06e1 */
