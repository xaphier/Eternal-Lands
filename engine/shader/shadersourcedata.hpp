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
			std::set<ShaderVersionType> m_versions;
			String m_source;

			void load_parameters_xml(const String &source,
				const xmlNodePtr node);

		public:
			ShaderSourceData();
			ShaderSourceData(const String &source,
				const xmlNodePtr node);
			~ShaderSourceData() throw();
			void load_xml(const String &source,
				const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;
			void build_source(
				const ShaderSourceParameterVector &locals, 
				const String &indent, OutStream &stream,
				ShaderSourceParameterVector &globals) const;
			bool check_source_parameter(const String &name) const;
			void set_parameters(
				const ShaderSourceParameterVector &parameters);

			inline void set_source(const String &source)
			{
				m_source = source;
			}

			inline void add_version(const ShaderVersionType version)
			{
				m_versions.insert(version);
			}

			inline void erase_version(
				const ShaderVersionType version)
			{
				m_versions.erase(version);
			}

			inline void set_glsl_120(const bool glsl_120)
			{
				if (glsl_120)
				{
					add_version(svt_120);
				}
				else
				{
					erase_version(svt_120);
				}
			}

			inline void set_glsl_130(const bool glsl_130)
			{
				if (glsl_130)
				{
					add_version(svt_130);
				}
				else
				{
					erase_version(svt_130);
				}
			}

			inline void set_glsl_140(const bool glsl_140)
			{
				if (glsl_140)
				{
					add_version(svt_140);
				}
				else
				{
					erase_version(svt_140);
				}
			}

			inline void set_glsl_150(const bool glsl_150)
			{
				if (glsl_150)
				{
					add_version(svt_150);
				}
				else
				{
					erase_version(svt_150);
				}
			}

			inline void set_glsl_330(const bool glsl_330)
			{
				if (glsl_330)
				{
					add_version(svt_330);
				}
				else
				{
					erase_version(svt_330);
				}
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

			inline bool get_version(
				const ShaderVersionType version) const
			{
				return m_versions.count(version) > 0;
			}

			inline bool get_glsl_120() const
			{
				return m_versions.count(svt_120) > 0;
			}

			inline bool get_glsl_130() const
			{
				return m_versions.count(svt_130) > 0;
			}

			inline bool get_glsl_140() const
			{
				return m_versions.count(svt_140) > 0;
			}

			inline bool get_glsl_150() const
			{
				return m_versions.count(svt_150) > 0;
			}

			inline bool get_glsl_330() const
			{
				return m_versions.count(svt_330) > 0;
			}

	};

}

#endif	/* UUID_e6b071c9_e635_4064_9341_86fb71c08a40 */
