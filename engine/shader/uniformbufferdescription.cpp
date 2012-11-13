/****************************************************************************
 *            uniformbufferdescription.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uniformbufferdescription.hpp"
#include "uniformdescription.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	UniformBufferDescription::UniformBufferDescription(
		const UniformBufferType type): m_type(type), m_size(0)
	{
		Uint32 size, array_stride, matrix_stride, count;

		BOOST_FOREACH(const AutoParameterType auto_parameter,
			UniformBufferUtil::get_auto_parameters(get_type()))
		{
			size = 0;
			array_stride = 0;
			matrix_stride = 0;

			switch (AutoParameterUtil::get_type(auto_parameter))
			{
				case pt_bvec4:
				case pt_ivec4:
				case pt_uvec4:
				case pt_vec4:
					size = 16;
					array_stride = 16;
					matrix_stride = 0;
					break;
				case pt_mat2x4:
					size = 32;
					array_stride = 32;
					matrix_stride = 16;
					break;
				case pt_mat3x4:
					size = 48;
					array_stride = 48;
					matrix_stride = 16;
					break;
				case pt_mat4x4:
					size = 64;
					array_stride = 64;
					matrix_stride = 16;
					break;
				default:
					assert(false);
					break;
			};

			count = AutoParameterUtil::get_max_size(auto_parameter);

			if (count == 1)
			{
				array_stride = 0;
			}

			m_auto_parameters.insert(std::pair<AutoParameterType,
				Uint32>(auto_parameter, m_uniforms.size()));

			m_uniforms.push_back(UniformDescription(
				auto_parameter, m_size, count, array_stride,
				matrix_stride, false));

			m_size += size * count;
		}
	}

	UniformBufferDescription::~UniformBufferDescription() noexcept
	{
	}

	void UniformBufferDescription::write(const String &indent,
		OutStream &str) const
	{
		str << indent << UTF8("layout(std140) uniform ");
		str << UniformBufferUtil::get_identifier(get_type());
		str << UTF8("\n") << indent << UTF8("{\n");

		BOOST_FOREACH(const UniformDescription &uniform, get_uniforms())
		{
			uniform.write(String(indent.get() + UTF8("\t")), str);
		}

		str << indent << UTF8("};\n");
	}

	void UniformBufferDescription::log(OutStream &str) const
	{
		str << UTF8("UniformBufferDescription");
		str << UTF8(" [type]: ") << get_type();
		str << UTF8(" [size]: ") << get_size();
		str << std::endl;

		BOOST_FOREACH(const UniformDescription &uniform, get_uniforms())
		{
			uniform.log(str);
			str << std::endl;
		}
	}

	const UniformDescription
		&UniformBufferDescription::get_uniform_description(
			const AutoParameterType auto_parameter)	const
	{
		AutoParameterTypeUint32Map::const_iterator found;
		String name;

		found = m_auto_parameters.find(auto_parameter);

		if (found == m_auto_parameters.end())
		{
			name = AutoParameterUtil::get_str(auto_parameter);

			EL_THROW_EXCEPTION(ItemNotFoundException()
				<< errinfo_item_name(name)
				<< errinfo_name(get_name()));
		}

		assert(m_uniforms.size() > found->second);

		return m_uniforms[found->second];
	}

	bool UniformBufferDescription::get_uniform_description(
		const AutoParameterType auto_parameter,
		UniformDescription &uniform_description) const
	{
		AutoParameterTypeUint32Map::const_iterator found;

		found = m_auto_parameters.find(auto_parameter);

		if (found == m_auto_parameters.end())
		{
			return false;
		}

		uniform_description = m_uniforms[found->second];

		return true;
	}

}
