/****************************************************************************
 *            shadersourcedata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e6b071c9_e635_4064_9341_86fb71c08a40
#define	UUID_e6b071c9_e635_4064_9341_86fb71c08a40

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "parametersizeutil.hpp"
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
			ShaderVersionType m_version;
			String m_source;

			void load_parameters_xml(const String &source,
				const xmlNodePtr node);
			void build_function_source(
				const ShaderSourceParameterVector &locals,
				const String &indent, const String &name,
				OutStream &stream,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			void build_function_use(const String &indent,
				const String &name,
				const String &parameter_prefix,
				OutStream &stream) const;

		public:
			ShaderSourceData();
			ShaderSourceData(const String &source,
				const xmlNodePtr node);
			~ShaderSourceData() noexcept;
			void load_xml(const String &source,
				const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;
			void build_source(
				const ShaderSourceParameterVector &locals, 
				const String &indent, OutStream &stream,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			bool check_source_parameter(const String &name) const;
			void set_parameters(
				const ShaderSourceParameterVector &parameters);
			void build_function(
				const ShaderSourceParameterVector &locals,
				const String &indent, const String &name,
				const String &parameter_prefix,
				const String &use_indent, OutStream &stream,
				OutStream &function,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;

			inline void set_source(const String &source) noexcept
			{
				m_source = source;
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

			inline const String &get_source() const noexcept
			{
				return m_source;
			}

			inline ShaderVersionType get_version() const noexcept
			{
				return m_version;
			}

	};

}

#endif	/* UUID_e6b071c9_e635_4064_9341_86fb71c08a40 */
