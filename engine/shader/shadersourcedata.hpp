/****************************************************************************
 *            shadersourcedata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e6b071c9_e635_4064_9341_86fb71c08a40
#define	UUID_e6b071c9_e635_4064_9341_86fb71c08a40

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "parametersizeutil.hpp"
#include "shaderutil.hpp"
#include "shaderversionutil.hpp"

/**
 * @file
 * @brief The @c class ShaderSourceData.
 * This file contains the @c class ShaderSourceData.
 */
namespace eternal_lands
{

	class ShaderSourceData
	{
		private:
			ShaderSourceParameterVector m_parameters;
			ShaderTypeSet m_shader_types;
			ShaderVersionType m_version;
			String m_source;

			void load_parameters_xml(const String &source,
				const xmlNodePtr node);

		public:
			ShaderSourceData();
			ShaderSourceData(const String &source,
				const xmlNodePtr node);
			~ShaderSourceData() noexcept;
			void load_xml(const String &source,
				const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;
			void set_parameters(
				const ShaderSourceParameterVector &parameters);

			inline void set_source(const String &source) noexcept
			{
				m_source = source;
			}

			inline void set_shader_types(
				const ShaderTypeSet &shader_types) noexcept
			{
				m_shader_types = shader_types;
			}

			inline void set_version(const ShaderVersionType version)
				noexcept
			{
				m_version = version;
			}

			inline const ShaderSourceParameterVector
				&get_parameters() const noexcept
			{
				return m_parameters;
			}

			inline const ShaderTypeSet &get_shader_types() const
				noexcept
			{
				return m_shader_types;
			}

			inline const String &get_source() const noexcept
			{
				return m_source;
			}

			inline ShaderVersionType get_version() const noexcept
			{
				return m_version;
			}

			inline bool get_has_shader_type(
				const ShaderType shader_type) const
			{
				return get_shader_types().count(shader_type) > 0;
			}

	};

}

#endif	/* UUID_e6b071c9_e635_4064_9341_86fb71c08a40 */
