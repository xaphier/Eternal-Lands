/****************************************************************************
 *            shadersourcedescription.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f
#define	UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shadersource.hpp"
#include "shaderbuildutil.hpp"

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
	class ShaderSourceDescription
	{
		private:
			ShaderSourceTypeStringMap m_sources;
			ShaderBuildType m_shader_build_type;
			Uint16 m_light_count;

		public:
			ShaderSourceDescription(
				const ShaderSourceTypeStringMap &sources,
				const ShaderBuildType shader_build_type,
				const Uint16 light_count);

			/**
			 * Default destructor.
			 */
			~ShaderSourceDescription() throw();

			/**
			 * Returns the sources of the different shader parts.
			 */
			inline const ShaderSourceTypeStringMap &get_sources()
				const
			{
				return m_sources;
			}

			/**
			 * Returns the number of lights the shader uses.
			 */
			inline Uint16 get_light_count() const
			{
				return m_light_count;
			}

			/**
			 * Returns the shader build type.
			 */
			inline ShaderBuildType get_shader_build_type() const
			{
				return m_shader_build_type;
			}

	};

}

#endif	/* UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f */
