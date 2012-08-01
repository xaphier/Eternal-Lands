/****************************************************************************
 *            uniformbufferdescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_687c6606_8efe_4027_bef5_8082304c7bee
#define	UUID_687c6606_8efe_4027_bef5_8082304c7bee

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "uniformbufferutil.hpp"
#include "autoparameterutil.hpp"

/**
 * @file
 * @brief The @c class UniformBufferDescription.
 * This file contains the @c class UniformBufferDescription.
 */
namespace eternal_lands
{

	class UniformBufferDescription
	{
		private:
			typedef std::map<AutoParameterType, Uint32>
				AutoParameterTypeUint32Map;
			UniformDescriptionVector m_uniforms;
			AutoParameterTypeUint32Map m_auto_parameters;
			UniformBufferType m_type;
			Uint32 m_size;

		public:
			UniformBufferDescription(const UniformBufferType type);
			~UniformBufferDescription() noexcept;
			void write(const String &indent, OutStream &str) const;
			void log(OutStream &str) const;
			UniformDescription &get_uniform_description(
				const AutoParameterType auto_parameter);
			const UniformDescription &get_uniform_description(
				const AutoParameterType auto_parameter) const;
			bool get_uniform_description(
				const AutoParameterType auto_parameter,
				UniformDescription &uniform_description) const;
			inline bool has_auto_parameter(
				const AutoParameterType auto_parameter) const
				noexcept
			{
				return m_auto_parameters.find(auto_parameter)
					!= m_auto_parameters.end();
			}

			inline UniformBufferType get_type() const noexcept
			{
				return m_type;
			}

			inline const String &get_name() const
			{
				return UniformBufferUtil::get_str(get_type());
			}

			inline Uint32 get_size() const noexcept
			{
				return m_size;
			}

			inline const UniformDescriptionVector &get_uniforms()
				const
			{
				return m_uniforms;
			}

			inline void set_type(const UniformBufferType type)
				noexcept
			{
				m_type = type;
			}

			inline void set_size(const Uint32 size) noexcept
			{
				m_size = size;
			}

	};

}

#endif	/* UUID_687c6606_8efe_4027_bef5_8082304c7bee */
