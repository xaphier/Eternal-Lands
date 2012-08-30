/****************************************************************************
 *            shadersource.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_aec9bc8b_e0b2_4dfe_98b4_3114d6113c12
#define	UUID_aec9bc8b_e0b2_4dfe_98b4_3114d6113c12

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

	class ShaderSource
	{
		private:
			ShaderSourceType m_type;
			String m_name;
			ShaderSourceDataVector m_datas;
			boost::uuids::uuid m_uuid;

			ShaderSource();
			void load_datas_xml(const String &source,
				const xmlNodePtr node);

		public:
			ShaderSource(const boost::uuids::uuid &uuid);
			~ShaderSource() noexcept;
			void load_xml(const String &file_name);
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			void save_xml(const String &file_name) const;
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;
			void load(const String &file_name);
			void build_source(const ShaderVersionType &version,
				const ShaderSourceParameterVector &locals, 
				const String &indent, OutStream &stream,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			void build_function(const ShaderVersionType &version,
				const ShaderSourceParameterVector &locals,
				const ParameterSizeTypeUint16Map &array_sizes,
				const String &indent,
				const String &parameter_prefix,
				const String &use_indent, OutStream &stream,
				OutStream &function,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			bool check_source_parameter(
				const ShaderVersionType &version,
				const String &name) const;
			bool get_has_data(const ShaderVersionType version)
				const;
			const ShaderSourceData &get_data(
				const ShaderVersionType version) const;
			void set_datas(const ShaderSourceDataVector &datas);
			String get_typed_name() const;

			inline void set_type(const ShaderSourceType type)
				noexcept
			{
				m_type = type;
			}

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
			}

			inline const ShaderSourceDataVector &get_datas() const
				noexcept
			{
				return m_datas;
			}

			inline ShaderSourceType get_type() const noexcept
			{
				return m_type;
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline const boost::uuids::uuid &get_uuid() const
				noexcept
			{
				return m_uuid;
			}

	};

}

#endif	/* UUID_aec9bc8b_e0b2_4dfe_98b4_3114d6113c12 */
