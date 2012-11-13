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
				const ShaderType shader_type,
				const ShaderVersionType version) const
				override;
			virtual String get_source(const ShaderType shader_type,
				const ShaderVersionType version) const
				override;
			virtual void do_load_xml(const xmlNodePtr node)
				override;
			virtual void do_save_xml(
				const XmlWriterSharedPtr &xml_writer) const
				override;
			void load_datas_xml(const String &source,
				const xmlNodePtr node);

		public:
			ShaderSource();
			virtual ~ShaderSource() noexcept override;
			virtual bool get_has_data(const ShaderType shader_type,
				const ShaderVersionType version) const
				override;
			const ShaderSourceData &get_data(
				const ShaderType shader_type,
				const ShaderVersionType version) const;
			void set_datas(const ShaderSourceDataVector &datas);
			static String get_xml_id();
			virtual ShaderSourceType get_type() const override;

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
