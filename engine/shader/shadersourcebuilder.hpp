/****************************************************************************
 *            shadersourcebuilder.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_27b60798_724e_424d_84d5_b06955bb406f
#define	UUID_27b60798_724e_424d_84d5_b06955bb406f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shadersource.hpp"
#include "parametersizeutil.hpp"
#include "commonparameterutil.hpp"
#include "shaderbuildutil.hpp"

/**
 * @file
 * @brief The @c class ShaderSourceBuilder.
 * This file contains the @c class ShaderSourceBuilder.
 */
namespace eternal_lands
{

	class ShaderSourceBuilder
	{
		private:
			class ShaderSourceBuildData;
			class ShaderSourceOptimizer;

			typedef std::pair<ShaderSourceType, String>
				ShaderSourceTypeStringPair;
			typedef boost::ptr_map<ShaderSourceTypeStringPair,
					ShaderSource>
				ShaderSourceTypeStringPairShaderSourceMap;

			ShaderSourceTypeStringPairShaderSourceMap
				m_shader_sources;
			ShaderSourceTypeStringMap m_sources;
			const GlobalVarsSharedPtr m_global_vars;
			FileSystemWeakPtr m_file_system;
			boost::scoped_ptr<ShaderSourceOptimizer> m_optimizer;
			float m_shadow_scale;
			Uint16 m_vertex_light_count;
			Uint16 m_fragment_light_count;
			Uint16 m_bone_count;
			bool m_dynamic_light_count;

			inline FileSystemSharedPtr get_file_system() const
			{
				FileSystemSharedPtr result;

				result = m_file_system.lock();

				assert(result.get() != 0);

				return result;
			}

			bool check_function(const ShaderSourceBuildData &data,
				const String &name,
				const ShaderSourceType shader_source_type)
				const;
			bool get_source_parameter(
				const ShaderSourceBuildData &data,
				const CommonParameterType common_parameter)
				const;
			void build_lights(
				const ShaderSourceBuildData &data,
				const ParameterSizeTypeUint16Map &array_sizes,
				const ShaderSourceParameterVector &locals, 
				const bool vertex, const bool shadow,
				OutStream &main,
				ShaderSourceParameterVector &globals,
				StringVariantMap &values) const;
			bool build_function(
				const ShaderSourceBuildData &data,
				const ParameterSizeTypeUint16Map &array_sizes,
				const ShaderSourceParameterVector &locals, 
				const ShaderSourceType shader_source_type,
				OutStream &stream,
				ShaderSourceParameterVector &globals,
				StringVariantMap &values) const;
			void load(Lua &lua, const String &name,
				ShaderSourceTypeStringMap &types) const;
			void build_vertex_source(
				const ShaderSourceBuildData &data,
				const ParameterSizeTypeUint16Map &array_sizes,
				OutStream &main,
				ShaderSourceParameterVector &globals,
				StringVariantMap &values) const;
			void build_fragment_source(
				const ShaderSourceBuildData &data,
				const ParameterSizeTypeUint16Map &array_sizes,
				OutStream &main,
				ShaderSourceParameterVector &globals,
				StringVariantMap &values) const;
			void load_file(const String &file_name);
			ShaderSourceTypeStringMap build_sources(
				const ShaderSourceDescription &description)
				const;
			bool check(const ShaderSourceTypeStringPair &source,
				const ShaderVersionType data_type) const;

		public:
			ShaderSourceBuilder(
				const GlobalVarsSharedPtr &global_vars,
				const FileSystemWeakPtr &file_system);
			~ShaderSourceBuilder() throw();
			void load(const String &file_name);
			void load_default(const String &file_name);
			void build(const Uint16 light_count,
				const ShaderBuildType shader_build_type,
				const ShaderSourceDescription &description,
				StringType &vertex, StringType &fragment,
				StringVariantMap &values) const;
			void set_shadow_map_type(const String &name);

			inline const ShaderSourceTypeStringMap &get_sources()
				const
			{
				return m_sources;
			}

			inline const GlobalVarsSharedPtr &get_global_vars()
				const
			{
				return m_global_vars;
			}

			/**
			 * Returns the darkening of the diffuse light when it's
			 * in shadow the shader uses.
			 */
			inline float get_shadow_scale() const
			{
				return m_shadow_scale;
			}

			/**
			 * Returns the number of vertex lights the shader uses.
			 */
			inline Uint16 get_vertex_light_count() const
			{
				return m_vertex_light_count;
			}

			/**
			 * Returns the number of fragment lights the shader
			 * uses.
			 */
			inline Uint16 get_fragment_light_count() const
			{
				return m_fragment_light_count;
			}

			/**
			 * Returns the number of fragment lights the shader
			 * uses.
			 */
			inline Uint16 get_light_count() const
			{
				return get_fragment_light_count() +
					get_vertex_light_count();
			}

			/**
			 * Returns the number of bones the shader uses.
			 */
			inline Uint16 get_bone_count() const
			{
				return m_bone_count;
			}

			/**
			 * Returns true if dynamic flow control should be used
			 * for fragment lights, else false.
			 */
			inline bool get_dynamic_light_count() const
			{
				return m_dynamic_light_count;
			}

	};

}

#endif	/* UUID_27b60798_724e_424d_84d5_b06955bb406f */
