/****************************************************************************
 *            shadersourcedescriptionloader.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_d5fad760_0ab2_48d2_8aa2_9b47feaab68a
#define	UUID_d5fad760_0ab2_48d2_8aa2_9b47feaab68a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shadersourcedescription.hpp"

/**
 * @file
 * @brief The @c class ShaderSourceDescription.
 * This file contains the @c class ShaderSourceDescription.
 */
namespace eternal_lands
{

	/**
	 * @brief This class describes a shader source.
	 *
	 * A shader source description is defined throught its sources,
	 * lighting informations, shadows, fog etc. All these informations are
	 * used to build the shader source that is used to compile and link
	 * the opengl shader.
	 */
	class ShaderSourceDescriptionLoader
	{
		private:
			ShaderSourceTypeStringMap m_sources;

			void load(const ShaderSourceTypeStringMap &defaults,
				const String &quality, const String &name,
				Lua &lua);
			void load_bool_or_string(
				const ShaderSourceTypeStringMap &defaults,
				const String &name,
				const ShaderSourceType shader_source_type,
				Lua &lua);
			void load_bool(
				const ShaderSourceTypeStringMap &defaults,
				const String &name,
				const ShaderSourceType shader_source_type,
				Lua &lua);
			void load_string(
				const ShaderSourceTypeStringMap &defaults,
				const String &name,
				const ShaderSourceType shader_source_type,
				Lua &lua);

		public:
			ShaderSourceDescriptionLoader(
				const GlobalVarsSharedPtr &global_vars,
				const ShaderSourceTypeStringMap &defaults,
				const String &name, const String &quality,
				Lua &lua);

			/**
			 * Default destructor.
			 */
			~ShaderSourceDescriptionLoader() throw();

			ShaderSourceDescription get_shader_source_description(
				const ShaderBuildType shader_build_type,
				const Uint16 light_count) const;

			/**
			 * Returns the sources of the different shader parts.
			 */
			inline const ShaderSourceTypeStringMap &get_sources()
				const
			{
				return m_sources;
			}

	};

}

#endif	/* UUID_d5fad760_0ab2_48d2_8aa2_9b47feaab68a */
