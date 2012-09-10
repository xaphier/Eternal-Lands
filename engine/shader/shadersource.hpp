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
#include "abstractshadersource.hpp"

/**
 * @file
 * @brief The @c class ShaderSource.
 * This file contains the @c class ShaderSource.
 */
namespace eternal_lands
{

	class ShaderSource: public AbstractShaderSource
	{
		private:
			ShaderSourceType m_type;
			ShaderSourceDataVector m_datas;

			virtual ShaderSourceParameterVector get_parameters(
				const ShaderVersionType version) const;
			virtual String get_source(
				const ShaderVersionType version) const;
			virtual void load_xml_node(const xmlNodePtr node);
			void load_datas_xml(const String &source,
				const xmlNodePtr node);

		public:
			ShaderSource();
			virtual ~ShaderSource() noexcept;
			void save_xml(const String &file_name) const;
			void save_xml(const XmlWriterSharedPtr &writer) const;
			virtual bool get_has_data(
				const ShaderVersionType version) const;
			const ShaderSourceData &get_data(
				const ShaderVersionType version) const;
			void set_datas(const ShaderSourceDataVector &datas);
			static String get_xml_id();
			virtual ShaderSourceType get_type() const;

			inline void set_type(const ShaderSourceType type)
				noexcept
			{
				m_type = type;
			}

			inline const ShaderSourceDataVector &get_datas() const
				noexcept
			{
				return m_datas;
			}

	};

}

#endif	/* UUID_aec9bc8b_e0b2_4dfe_98b4_3114d6113c12 */
