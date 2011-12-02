/****************************************************************************
 *            glmutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "glmutil.hpp"

namespace eternal_lands
{

	OutStream& operator<<(OutStream &str, const glm::vec2 &value)
	{
		str << value[0];
		str << L" " << value[1];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::vec3 &value)
	{
		str << value[0];
		str << L" " << value[1];
		str << L" " << value[2];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::vec4 &value)
	{
		str << value[0];
		str << L" " << value[1];
		str << L" " << value[2];
		str << L" " << value[3];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::bvec2 &value)
	{
		str << value[0];
		str << L" " << value[1];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::bvec3 &value)
	{
		str << value[0];
		str << L" " << value[1];
		str << L" " << value[2];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::bvec4 &value)
	{
		str << value[0];
		str << L" " << value[1];
		str << L" " << value[2];
		str << L" " << value[3];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::ivec2 &value)
	{
		str << value[0];
		str << L" " << value[1];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::ivec3 &value)
	{
		str << value[0];
		str << L" " << value[1];
		str << L" " << value[2];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::ivec4 &value)
	{
		str << value[0];
		str << L" " << value[1];
		str << L" " << value[2];
		str << L" " << value[3];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::uvec2 &value)
	{
		str << value[0];
		str << L" " << value[1];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::uvec3 &value)
	{
		str << value[0];
		str << L" " << value[1];
		str << L" " << value[2];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::uvec4 &value)
	{
		str << value[0];
		str << L" " << value[1];
		str << L" " << value[2];
		str << L" " << value[3];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::mat2x2 &value)
	{
		str << value[0][0];
		str << L" " << value[0][1];
		str << L" " << value[1][0];
		str << L" " << value[1][1];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::mat2x3 &value)
	{
		str << value[0][0];
		str << L" " << value[0][1];
		str << L" " << value[0][2];
		str << L" " << value[1][0];
		str << L" " << value[1][1];
		str << L" " << value[1][2];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::mat2x4 &value)
	{
		str << value[0][0];
		str << L" " << value[0][1];
		str << L" " << value[0][2];
		str << L" " << value[0][3];
		str << L" " << value[1][0];
		str << L" " << value[1][1];
		str << L" " << value[1][2];
		str << L" " << value[1][3];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::mat3x2 &value)
	{
		str << value[0][0];
		str << L" " << value[0][1];
		str << L" " << value[1][0];
		str << L" " << value[1][1];
		str << L" " << value[2][0];
		str << L" " << value[2][1];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::mat3x3 &value)
	{
		str << value[0][0];
		str << L" " << value[0][1];
		str << L" " << value[0][2];
		str << L" " << value[1][0];
		str << L" " << value[1][1];
		str << L" " << value[1][2];
		str << L" " << value[2][0];
		str << L" " << value[2][1];
		str << L" " << value[2][2];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::mat3x4 &value)
	{
		str << value[0][0];
		str << L" " << value[0][1];
		str << L" " << value[0][2];
		str << L" " << value[0][3];
		str << L" " << value[1][0];
		str << L" " << value[1][1];
		str << L" " << value[1][2];
		str << L" " << value[1][3];
		str << L" " << value[2][0];
		str << L" " << value[2][1];
		str << L" " << value[2][2];
		str << L" " << value[2][3];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::mat4x2 &value)
	{
		str << value[0][0];
		str << L" " << value[0][1];
		str << L" " << value[1][0];
		str << L" " << value[1][1];
		str << L" " << value[2][0];
		str << L" " << value[2][1];
		str << L" " << value[3][0];
		str << L" " << value[3][1];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::mat4x3 &value)
	{
		str << value[0][0];
		str << L" " << value[0][1];
		str << L" " << value[0][2];
		str << L" " << value[1][0];
		str << L" " << value[1][1];
		str << L" " << value[1][2];
		str << L" " << value[2][0];
		str << L" " << value[2][1];
		str << L" " << value[2][2];
		str << L" " << value[3][0];
		str << L" " << value[3][1];
		str << L" " << value[3][2];

		return str;
	}

	OutStream& operator<<(OutStream &str, const glm::mat4x4 &value)
	{
		str << value[0][0];
		str << L" " << value[0][1];
		str << L" " << value[0][2];
		str << L" " << value[0][3];
		str << L" " << value[1][0];
		str << L" " << value[1][1];
		str << L" " << value[1][2];
		str << L" " << value[1][3];
		str << L" " << value[2][0];
		str << L" " << value[2][1];
		str << L" " << value[2][2];
		str << L" " << value[2][3];
		str << L" " << value[3][0];
		str << L" " << value[3][1];
		str << L" " << value[3][2];
		str << L" " << value[3][3];

		return str;
	}

	InStream& operator>>(InStream &str, glm::vec2 &value)
	{
		str >> value[0];
		str >> value[1];

		return str;
	}

	InStream& operator>>(InStream &str, glm::vec3 &value)
	{
		str >> value[0];
		str >> value[1];
		str >> value[2];

		return str;
	}

	InStream& operator>>(InStream &str, glm::vec4 &value)
	{
		str >> value[0];
		str >> value[1];
		str >> value[2];
		str >> value[3];

		return str;
	}

	InStream& operator>>(InStream &str, glm::bvec2 &value)
	{
		str >> value[0];
		str >> value[1];

		return str;
	}

	InStream& operator>>(InStream &str, glm::bvec3 &value)
	{
		str >> value[0];
		str >> value[1];
		str >> value[2];

		return str;
	}

	InStream& operator>>(InStream &str, glm::bvec4 &value)
	{
		str >> value[0];
		str >> value[1];
		str >> value[2];
		str >> value[3];

		return str;
	}

	InStream& operator>>(InStream &str, glm::ivec2 &value)
	{
		str >> value[0];
		str >> value[1];

		return str;
	}

	InStream& operator>>(InStream &str, glm::ivec3 &value)
	{
		str >> value[0];
		str >> value[1];
		str >> value[2];

		return str;
	}

	InStream& operator>>(InStream &str, glm::ivec4 &value)
	{
		str >> value[0];
		str >> value[1];
		str >> value[2];
		str >> value[3];

		return str;
	}

	InStream& operator>>(InStream &str, glm::uvec2 &value)
	{
		str >> value[0];
		str >> value[1];

		return str;
	}

	InStream& operator>>(InStream &str, glm::uvec3 &value)
	{
		str >> value[0];
		str >> value[1];
		str >> value[2];

		return str;
	}

	InStream& operator>>(InStream &str, glm::uvec4 &value)
	{
		str >> value[0];
		str >> value[1];
		str >> value[2];
		str >> value[3];

		return str;
	}

	InStream& operator>>(InStream &str, glm::mat2x2 &value)
	{
		str >> value[0][0];
		str >> value[0][1];
		str >> value[1][0];
		str >> value[1][1];

		return str;
	}

	InStream& operator>>(InStream &str, glm::mat2x3 &value)
	{
		str >> value[0][0];
		str >> value[0][1];
		str >> value[0][2];
		str >> value[1][0];
		str >> value[1][1];
		str >> value[1][2];

		return str;
	}

	InStream& operator>>(InStream &str, glm::mat2x4 &value)
	{
		str >> value[0][0];
		str >> value[0][1];
		str >> value[0][2];
		str >> value[0][3];
		str >> value[1][0];
		str >> value[1][1];
		str >> value[1][2];
		str >> value[1][3];

		return str;
	}

	InStream& operator<<(InStream &str, glm::mat3x2 &value)
	{
		str >> value[0][0];
		str >> value[0][1];
		str >> value[1][0];
		str >> value[1][1];
		str >> value[2][0];
		str >> value[2][1];

		return str;
	}

	InStream& operator>>(InStream &str, glm::mat3x3 &value)
	{
		str >> value[0][0];
		str >> value[0][1];
		str >> value[0][2];
		str >> value[1][0];
		str >> value[1][1];
		str >> value[1][2];
		str >> value[2][0];
		str >> value[2][1];
		str >> value[2][2];

		return str;
	}

	InStream& operator>>(InStream &str, glm::mat3x4 &value)
	{
		str >> value[0][0];
		str >> value[0][1];
		str >> value[0][2];
		str >> value[0][3];
		str >> value[1][0];
		str >> value[1][1];
		str >> value[1][2];
		str >> value[1][3];
		str >> value[2][0];
		str >> value[2][1];
		str >> value[2][2];
		str >> value[2][3];

		return str;
	}

	InStream& operator>>(InStream &str, glm::mat4x2 &value)
	{
		str >> value[0][0];
		str >> value[0][1];
		str >> value[1][0];
		str >> value[1][1];
		str >> value[2][0];
		str >> value[2][1];
		str >> value[3][0];
		str >> value[3][1];

		return str;
	}

	InStream& operator>>(InStream &str, glm::mat4x3 &value)
	{
		str >> value[0][0];
		str >> value[0][1];
		str >> value[0][2];
		str >> value[1][0];
		str >> value[1][1];
		str >> value[1][2];
		str >> value[2][0];
		str >> value[2][1];
		str >> value[2][2];
		str >> value[3][0];
		str >> value[3][1];
		str >> value[3][2];

		return str;
	}

	InStream& operator>>(InStream &str, glm::mat4x4 &value)
	{
		str >> value[0][0];
		str >> value[0][1];
		str >> value[0][2];
		str >> value[0][3];
		str >> value[1][0];
		str >> value[1][1];
		str >> value[1][2];
		str >> value[1][3];
		str >> value[2][0];
		str >> value[2][1];
		str >> value[2][2];
		str >> value[2][3];
		str >> value[3][0];
		str >> value[3][1];
		str >> value[3][2];
		str >> value[3][3];

		return str;
	}

}
