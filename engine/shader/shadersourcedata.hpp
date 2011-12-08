/****************************************************************************
 *            shadersourcedata.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e6b071c9_e635_4064_9341_86fb71c08a40
#define	UUID_e6b071c9_e635_4064_9341_86fb71c08a40

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "parametersizeutil.hpp"

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
			String m_source;
			bool m_glsl_120;
			bool m_glsl_150;
			bool m_material_default;
			bool m_material_merged;

			void load_parameters_xml(const xmlNodePtr node);

		public:
			ShaderSourceData();
			ShaderSourceData(const xmlNodePtr node);
			~ShaderSourceData() throw();
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;
			void build_source(
				const ShaderSourceParameterVector &locals, 
				OutStream &stream,
				ShaderSourceParameterVector &globals) const;
			bool check_source_parameter(const String &name) const;
			void set_parameters(
				const ShaderSourceParameterVector &parameters);

			inline void set_source(const String &source)
			{
				m_source = source;
			}

			inline void set_glsl_120(const bool glsl_120)
			{
				m_glsl_120 = glsl_120;
			}

			inline void set_glsl_150(const bool glsl_150)
			{
				m_glsl_150 = glsl_150;
			}

			inline void set_material_default(
				const bool material_default)
			{
				m_material_default = material_default;
			}

			inline void set_material_merged(
				const bool material_merged)
			{
				m_material_merged = material_merged;
			}

			inline const ShaderSourceParameterVector
				&get_parameters() const
			{
				return m_parameters;
			}

			inline const String &get_source() const
			{
				return m_source;
			}

			inline bool get_glsl_120() const
			{
				return m_glsl_120;
			}

			inline bool get_glsl_150() const
			{
				return m_glsl_150;
			}

			inline bool get_material_default() const
			{
				return m_material_default;
			}

			inline bool get_material_merged() const
			{
				return m_material_merged;
			}

	};

}

#endif	/* UUID_e6b071c9_e635_4064_9341_86fb71c08a40 */
