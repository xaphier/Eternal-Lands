/****************************************************************************
 *            shadersourceterrain.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_bdba47be_bf5a_4315_b031_ea6027e40175
#define	UUID_bdba47be_bf5a_4315_b031_ea6027e40175

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shaderblendutil.hpp"
#include "abstractshadersource.hpp"

/**
 * @file
 * @brief The @c class ShaderSourceTerrain.
 * This file contains the @c class ShaderSourceTerrain.
 */
namespace eternal_lands
{

	class ShaderSourceTerrain: public AbstractShaderSource
	{
		private:
			typedef std::vector<ShaderBlendData>
				ShaderBlendDataVector;

			ShaderBlendDataVector m_data;

			void write_blend(const String &indent,
				const Uint16 index, OutStream &str) const;
			void write_blend_fetch(const String &indent,
				const Uint16 index, const bool use_glsl_130,
				OutStream &str);
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;
			virtual ShaderSourceParameterVector get_parameters(
				const ShaderVersionType version) const = 0;
			virtual String get_source(
				const ShaderVersionType version) const = 0;

		public:
			ShaderSourceTerrain();
			~ShaderSourceTerrain() noexcept;
			void load_xml(const String &file_name);
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			void save_xml(const String &file_name) const;
			virtual ShaderSourceType get_type() const;
			virtual bool get_has_data(
				const ShaderVersionType version) const;

	};

}

#endif	/* UUID_bdba47be_bf5a_4315_b031_ea6027e40175 */
