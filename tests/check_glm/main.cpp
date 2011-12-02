/****************************************************************************
 *            main.cpp
 *
 * Author: 2010  Daniel Jungmann <dsj@gmx.net>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include <glm/glm.hpp>

void check_matrix_sizes()
{
	assert(glm::mat2x2::col_type::value_size() == glm::mat2x2::col_size());
	assert(glm::mat2x2::row_type::value_size() == glm::mat2x2::row_size());
	assert(glm::mat2x3::col_type::value_size() == glm::mat2x3::col_size());
	assert(glm::mat2x3::row_type::value_size() == glm::mat2x3::row_size());
	assert(glm::mat2x4::col_type::value_size() == glm::mat2x4::col_size());
	assert(glm::mat2x4::row_type::value_size() == glm::mat2x4::row_size());

	assert(glm::mat3x2::col_type::value_size() == glm::mat3x2::col_size());
	assert(glm::mat3x2::row_type::value_size() == glm::mat3x2::row_size());
	assert(glm::mat3x3::col_type::value_size() == glm::mat3x3::col_size());
	assert(glm::mat3x3::row_type::value_size() == glm::mat3x3::row_size());
	assert(glm::mat3x4::col_type::value_size() == glm::mat3x4::col_size());
	assert(glm::mat3x4::row_type::value_size() == glm::mat3x4::row_size());

	assert(glm::mat4x2::col_type::value_size() == glm::mat4x2::col_size());
	assert(glm::mat4x2::row_type::value_size() == glm::mat4x2::row_size());
	assert(glm::mat4x3::col_type::value_size() == glm::mat4x3::col_size());
	assert(glm::mat4x3::row_type::value_size() == glm::mat4x3::row_size());
	assert(glm::mat4x4::col_type::value_size() == glm::mat4x4::col_size());
	assert(glm::mat4x4::row_type::value_size() == glm::mat4x4::row_size());
}

int main(int argc, char *argv[])
{
	glm::mat2x2 m2x2;
	glm::mat2x3 m2x3;
	glm::mat2x4 m2x4;
	glm::mat3x2 m3x2;
	glm::mat3x3 m3x3;
	glm::mat3x4 m3x4;
	glm::mat4x2 m4x2;
	glm::mat4x3 m4x3;
	glm::mat4x4 m4x4;
	glm::vec4 v4;
	glm::vec3 v3;
	glm::vec2 v2;
	unsigned int i;

	check_matrix_sizes();

	for (i = 0; i < 2; i++)
	{
		v2 += m2x2[i];
		v3 += m2x3[i];
		v4 += m2x4[i];
	}

	v2 += m2x2 * v2;
	v3 += m2x3 * v2;
	v4 += m2x4 * v2;

	v2 += v2 * m2x2;
	v2 += v3 * m2x3;
	v2 += v4 * m2x4;

	for (i = 0; i < 3; i++)
	{
		v2 += m3x2[i];
		v3 += m3x3[i];
		v4 += m3x4[i];
	}

	v2 += m3x2 * v3;
	v3 += m3x3 * v3;
	v4 += m3x4 * v3;

	v3 += v2 * m3x2;
	v3 += v3 * m3x3;
	v3 += v4 * m3x4;

	for (i = 0; i < 4; i++)
	{
		v2 += m4x2[i];
		v3 += m4x3[i];
		v4 += m4x4[i];
	}

	v2 += m4x2 * v4;
	v3 += m4x3 * v4;
	v4 += m4x4 * v4;

	v4 += v2 * m4x2;
	v4 += v3 * m4x3;
	v4 += v4 * m4x4;

	return 0;
}
