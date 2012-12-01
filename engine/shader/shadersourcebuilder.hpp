/****************************************************************************
 *            shadersourcebuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_27b60798_724e_424d_84d5_b06955bb406f
#define	UUID_27b60798_724e_424d_84d5_b06955bb406f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractshadersource.hpp"
#include "parametersizeutil.hpp"
#include "commonparameterutil.hpp"
#include "shaderbuildutil.hpp"
#include "shaderoutpututil.hpp"
#include "shaderutil.hpp"

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
			typedef std::map<ShaderSourceTypeStringPair,
					AbstractShaderSourceSharedPtr>
				ShaderSourceSharedPrMap;

			ShaderSourceSharedPrMap m_shader_sources;
			ShaderSourceTypeStringMap m_default_sources;
			const GlobalVarsSharedPtr m_global_vars;
			const FileSystemSharedPtr m_file_system;
			const UniformBufferDescriptionCacheWeakPtr
				m_uniform_buffer_description_cache;
			boost::scoped_ptr<ShaderSourceOptimizer> m_optimizer;
			float m_shadow_scale;
			Uint16 m_vertex_lights_count;
			Uint16 m_fragment_lights_count;
			Uint16 m_bones_count;
			bool m_dynamic_lights_count;

			inline const GlobalVarsSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

			inline const FileSystemSharedPtr &get_file_system()
				const noexcept
			{
				return m_file_system;
			}

			inline UniformBufferDescriptionCacheSharedPtr
				get_uniform_buffer_description_cache() const
				noexcept
			{
				UniformBufferDescriptionCacheSharedPtr result;

				result = m_uniform_buffer_description_cache.
					lock();

				assert(result.get() != nullptr);

				return result;
			}

			/**
			 * Converts from Lambert Azimuthal Equal-Area
			 * projection. Optimized for Uint8 values (0..255)
			 * @param value The Lambert Azimuthal Equal-Area
			 * projection vector to use (0..1).
			 * @return The normalized vector.
			 */
			void build_decode_normal(const String &indent,
				OutStream &functions) const;

			/**
			 * Convertes the normalized rgb10_a2 vector to the
			 * scaled terrain displacement vector.
			 */
			void build_decode_terrain_displacement(
				const String &indent, OutStream &functions)
				const;

			/**
			 * Checks if the given shader source is loaded here or
			 * in the data, returns true and set the shader_source.
			 * If the shader source is not loaded, it tries to load
			 * it and add it to the build data, returns true and
			 * set the shader_source. If that fails, it returns
			 * flase.
			 */
			bool get_shader_source(
				const ShaderSourceType shader_source_type,
				ShaderSourceBuildData &build_data,
				AbstractShaderSourceSharedPtr &shader_source)
				const;

			bool check_function(const String &name,
				const ShaderSourceType shader_source_type,
				const ShaderType shader_type,
				ShaderSourceBuildData &build_data) const;
			bool get_source_parameter(
				const CommonParameterType common_parameter,
				ShaderSourceBuildData &build_data) const;
			void build_lights(
				const ParameterSizeTypeUint16Map &array_sizes,
				const ShaderSourceParameterVector &locals, 
				const String &indent,
				const ShaderType shader_type, const bool shadow,
				ShaderSourceBuildData &build_data,
				OutStream &main, OutStream &functions,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			void build_light_index_lights(
				const ParameterSizeTypeUint16Map &array_sizes,
				const ShaderSourceParameterVector &locals, 
				const String &indent, const bool shadow,
				ShaderSourceBuildData &build_data,
				OutStream &main, OutStream &functions,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			void build_light_index_x4_lights(
				const ShaderSourceBuildData &build_data,
				const String &indent, const String &loop_indent,
				OutStream &stream) const;
			void build_light_index_x5_lights(
				const ShaderSourceBuildData &build_data,
				const String &indent, const String &loop_indent,
				OutStream &stream) const;
			void build_light_index_x8_lights(
				const ShaderSourceBuildData &build_data,
				const String &indent, const String &loop_indent,
				OutStream &stream) const;
			bool build_function(
				const ParameterSizeTypeUint16Map &array_sizes,
				const ShaderSourceParameterVector &locals, 
				const String &indent,
				const ShaderSourceType shader_source_type,
				const ShaderType shader_type,
				ShaderSourceBuildData &build_data,
				OutStream &stream, OutStream &functions,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			void build_vertex_source(
				const ParameterSizeTypeUint16Map &array_sizes,
				ShaderSourceBuildData &build_data,
				OutStream &main, OutStream &functions,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			void build_geometry_source(
				const ParameterSizeTypeUint16Map &array_sizes,
				const ShaderSourceParameterVector &varyings,
				const String &in_prefix,
				const String &out_prefix, const bool use_blocks,
				ShaderSourceBuildData &build_data,
				OutStream &main, OutStream &functions,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			void build_fragment_source(
				const ParameterSizeTypeUint16Map &array_sizes,
				ShaderSourceBuildData &build_data,
				OutStream &main, OutStream &functions,
				ShaderSourceParameterVector &globals,
				UniformBufferUsage &uniform_buffers) const;
			void load_shader_source(const String &file_name,
				AbstractShaderSourceSharedPtr &shader_source)
				const;
			bool load_shader_source(const String &file_name);
			ShaderSourceTypeStringMap build_sources(
				const EffectDescription &description) const;
			void load_xml(const xmlNodePtr node);
			void load_sources(const xmlNodePtr node);

		public:
			ShaderSourceBuilder(
				const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system,
				const UniformBufferDescriptionCacheWeakPtr
					&uniform_buffer_description_cache);
			~ShaderSourceBuilder() noexcept;
			void load_xml(const String &file_name);
			void load_shader_sources(const String &dir);
			void build(const EffectDescription &description,
				const ShaderBuildType shader_build,
				const ShaderOutputType shader_output,
				const Uint16 lights_count,
				ShaderTypeStringMap &program_description) const;
			void set_shadow_map_type(const String &name);
			StringVector get_shader_source_names(
				const ShaderSourceType shader_source) const;

			inline const ShaderSourceTypeStringMap
				&get_default_sources() const noexcept
			{
				return m_default_sources;
			}

			/**
			 * Returns the darkening of the diffuse light when it's
			 * in shadow the shader uses.
			 */
			inline float get_shadow_scale() const noexcept
			{
				return m_shadow_scale;
			}

			/**
			 * Returns the number of vertex lights the shader uses.
			 */
			inline Uint16 get_vertex_lights_count() const noexcept
			{
				return m_vertex_lights_count;
			}

			/**
			 * Returns the number of fragment lights the shader
			 * uses.
			 */
			inline Uint16 get_fragment_lights_count() const noexcept
			{
				return m_fragment_lights_count;
			}

			/**
			 * Returns the number of fragment lights the shader
			 * uses.
			 */
			inline Uint16 get_lights_count() const noexcept
			{
				return get_fragment_lights_count() +
					get_vertex_lights_count();
			}

			/**
			 * Returns the number of bones the shader uses.
			 */
			inline Uint16 get_bones_count() const noexcept
			{
				return m_bones_count;
			}

			/**
			 * Returns true if dynamic flow control should be used
			 * for fragment lights, else false.
			 */
			inline bool get_dynamic_lights_count() const noexcept
			{
				return m_dynamic_lights_count;
			}

	};

}

#endif	/* UUID_27b60798_724e_424d_84d5_b06955bb406f */
