/****************************************************************************
 *            abstractshadersource.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c57bcef6_3ca1_4e42_8bf8_22df8c22e000
#define	UUID_c57bcef6_3ca1_4e42_8bf8_22df8c22e000

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shadersourceutil.hpp"
#include "parametersizeutil.hpp"
#include "shaderversionutil.hpp"

/**
 * @file
 * @brief The @c class ShaderSource.
 * This file contains the @c class ShaderSource.
 */
namespace eternal_lands
{

	class AbstractShaderSource: public boost::noncopyable
	{
		private:
			String m_name;

			virtual ShaderSourceParameterVector get_parameters(
				const ShaderVersionType version) const = 0;
			virtual String get_source(
				const ShaderVersionType version) const = 0;
			virtual void do_load_xml(const xmlNodePtr node) = 0;
			virtual void do_save_xml(
				const XmlWriterSharedPtr &writer) const = 0;

		protected:
			AbstractShaderSource();
			void build_function_source(
				const ShaderSourceParameterVector &locals,
				const ParameterSizeTypeUint16Map &sizes,
				const String &indent, const String &name,
				const ShaderVersionType version,
				OutStream &stream,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			void build_function_use(const String &indent,
				const String &name,
				const String &parameter_prefix,
				const ShaderVersionType version,
				OutStream &stream) const;

		public:
			virtual ~AbstractShaderSource() noexcept;
			void build_source(const ShaderVersionType version,
				const ShaderSourceParameterVector &locals, 
				const String &indent, OutStream &stream,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			void build_function(const ShaderVersionType version,
				const ShaderSourceParameterVector &locals,
				const ParameterSizeTypeUint16Map &array_sizes,
				const String &indent,
				const String &parameter_prefix,
				const String &use_indent, OutStream &stream,
				OutStream &function,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			bool check_source_parameter(
				const ShaderVersionType version,
				const String &name) const;
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			void load_xml(const String &file_name);
			void load_xml(const xmlNodePtr node);
			void load_xml_string(const String &str);
			void save_xml(const String &file_name) const;
			String save_xml_string() const;
			String get_typed_name() const;
			virtual ShaderSourceType get_type() const = 0;
			virtual bool get_has_data(
				const ShaderVersionType version) const = 0;

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

	};

}

#endif	/* UUID_aec9bc8b_e0b2_4dfe_98b4_3114d6113c12 */
