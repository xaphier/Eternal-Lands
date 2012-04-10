/****************************************************************************
 *            glmutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "glmutil.hpp"

namespace eternal_lands
{

	namespace
	{

		template <typename T>
		OutStream& print_quat(OutStream &str, const T &value)
		{
			Uint16 i;

			str << value[0];

			for (i = 1; i < 4; ++i)
			{
				str << UTF8(" ") << value[i];
			}

			return str;
		}

		template <typename T>
		OutStream& print_vec(OutStream &str, const T &value)
		{
			Uint16 i;

			str << value[0];

			for (i = 1; i < value.length(); ++i)
			{
				str << UTF8(" ") << value[i];
			}

			return str;
		}

		template <typename T>
		OutStream& print_bvec(OutStream &str, const T &value)
		{
			Uint16 i;

			if (value[0])
			{
				str << UTF8("true");
			}
			else
			{
				str << UTF8("false");
			}

			for (i = 1; i < value.length(); ++i)
			{
				if (value[i])
				{
					str << UTF8(" true");
				}
				else
				{
					str << UTF8(" false");
				}
			}

			return str;
		}

		template <typename T>
		OutStream& print_mat(OutStream &str, const T &value)
		{
			Uint16 i, j;

			for (i = 0; i < value.row_size(); ++i)
			{
				for (j = 0; j < value.col_size(); ++j)
				{
					if ((i > 0) || (j > 0))
					{
						str << UTF8(" ");
					}

					str << value[i][j];
				}
			}

			return str;
		}

		template <typename T>
		InStream& scan_quat(InStream &str, T &value)
		{
			Uint16 i;

			for (i = 0; i < 4; ++i)
			{
				str >> value[i];
			}

			return str;
		}

		template <typename T>
		InStream& scan_vec(InStream &str, T &value)
		{
			Uint16 i;

			for (i = 0; i < value.length(); ++i)
			{
				str >> value[i];
			}

			return str;
		}

		template <typename T>
		InStream& scan_bvec(InStream &str, T &value)
		{
			std::string tmp;
			Uint16 i;

			for (i = 0; i < value.length(); ++i)
			{
				str >> tmp;

				if ((tmp == UTF8("true")) || (tmp == UTF8("1"))
					|| (tmp == UTF8("yes")) ||
					(tmp == UTF8("1.0")))
				{
					value[i] = true;
				}
				else
				{
					value[i] = false;
				}
			}

			return str;
		}

		template <typename T>
		InStream& scan_mat(InStream &str, T &value)
		{
			Uint16 i, j;

			for (i = 0; i < value.row_size(); ++i)
			{
				for (j = 0; j < value.col_size(); ++j)
				{
					str >> value[i][j];
				}
			}

			return str;
		}

	}

	OutStream& operator<<(OutStream &str, const glm::quat &value)
	{
		return print_quat(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::vec2 &value)
	{
		return print_vec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::vec3 &value)
	{
		return print_vec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::vec4 &value)
	{
		return print_vec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::bvec2 &value)
	{
		return print_bvec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::bvec3 &value)
	{
		return print_bvec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::bvec4 &value)
	{
		return print_bvec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::ivec2 &value)
	{
		return print_vec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::ivec3 &value)
	{
		return print_vec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::ivec4 &value)
	{
		return print_vec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::uvec2 &value)
	{
		return print_vec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::uvec3 &value)
	{
		return print_vec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::uvec4 &value)
	{
		return print_vec(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::mat2x2 &value)
	{
		return print_mat(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::mat2x3 &value)
	{
		return print_mat(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::mat2x4 &value)
	{
		return print_mat(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::mat3x2 &value)
	{
		return print_mat(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::mat3x3 &value)
	{
		return print_mat(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::mat3x4 &value)
	{
		return print_mat(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::mat4x2 &value)
	{
		return print_mat(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::mat4x3 &value)
	{
		return print_mat(str, value);
	}

	OutStream& operator<<(OutStream &str, const glm::mat4x4 &value)
	{
		return print_mat(str, value);
	}

	InStream& operator>>(InStream &str, glm::quat &value)
	{
		return scan_quat(str, value);
	}

	InStream& operator>>(InStream &str, glm::vec2 &value)
	{
		return scan_vec(str, value);
	}

	InStream& operator>>(InStream &str, glm::vec3 &value)
	{
		return scan_vec(str, value);
	}

	InStream& operator>>(InStream &str, glm::vec4 &value)
	{
		return scan_vec(str, value);
	}

	InStream& operator>>(InStream &str, glm::bvec2 &value)
	{
		return scan_bvec(str, value);
	}

	InStream& operator>>(InStream &str, glm::bvec3 &value)
	{
		return scan_bvec(str, value);
	}

	InStream& operator>>(InStream &str, glm::bvec4 &value)
	{
		return scan_bvec(str, value);
	}

	InStream& operator>>(InStream &str, glm::ivec2 &value)
	{
		return scan_vec(str, value);
	}

	InStream& operator>>(InStream &str, glm::ivec3 &value)
	{
		return scan_vec(str, value);
	}

	InStream& operator>>(InStream &str, glm::ivec4 &value)
	{
		return scan_vec(str, value);
	}

	InStream& operator>>(InStream &str, glm::uvec2 &value)
	{
		return scan_vec(str, value);
	}

	InStream& operator>>(InStream &str, glm::uvec3 &value)
	{
		return scan_vec(str, value);
	}

	InStream& operator>>(InStream &str, glm::uvec4 &value)
	{
		return scan_vec(str, value);
	}

	InStream& operator>>(InStream &str, glm::mat2x2 &value)
	{
		return scan_mat(str, value);
	}

	InStream& operator>>(InStream &str, glm::mat2x3 &value)
	{
		return scan_mat(str, value);
	}

	InStream& operator>>(InStream &str, glm::mat2x4 &value)
	{
		return scan_mat(str, value);
	}

	InStream& operator>>(InStream &str, glm::mat3x2 &value)
	{
		return scan_mat(str, value);
	}

	InStream& operator>>(InStream &str, glm::mat3x3 &value)
	{
		return scan_mat(str, value);
	}

	InStream& operator>>(InStream &str, glm::mat3x4 &value)
	{
		return scan_mat(str, value);
	}

	InStream& operator>>(InStream &str, glm::mat4x2 &value)
	{
		return scan_mat(str, value);
	}

	InStream& operator>>(InStream &str, glm::mat4x3 &value)
	{
		return scan_mat(str, value);
	}

	InStream& operator>>(InStream &str, glm::mat4x4 &value)
	{
		return scan_mat(str, value);
	}

}
