/****************************************************************************
 *            shadersourcebuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourcebuilder.hpp"
#include "exceptions.hpp"
#include "autoparameterutil.hpp"
#include "vertexelement.hpp"
#include "logging.hpp"
#include "globalvars.hpp"
#include "effect/effectdescription.hpp"
#include "commonparameterutil.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "samplerparameterutil.hpp"
#include "filesystem.hpp"
#include "glsl_optimizer/glsl/glsl_optimizer.h"
#include "xmlreader.hpp"
#include "effect/effect.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "abstractterrain.hpp"
#include "glmutil.hpp"
#include "uniformbufferusage.hpp"
#include "colorcorrection.hpp"
#include "shadersource.hpp"
#include "shadersourceterrain.hpp"
#include "effect/effectnodes.hpp"

namespace eternal_lands
{

	namespace
	{

		enum ShaderSourceLocalType
		{
			sslt_diffuse_colors_sum,
			sslt_specular_colors_sum,
			sslt_shadow_values,
			sslt_i,
			sslt_index,
			sslt_packed_light_indices_4,
			sslt_floor_values_4,
			sslt_unpack_const_4,
			sslt_packed_light_indices_3,
			sslt_floor_values_3,
			sslt_unpack_const_3,
			sslt_light_indices,
			sslt_project_space,
			sslt_light_index
		};

		class ShaderSourceLocalTypeData
		{
			private:
				const String m_name;
				const ParameterType m_type;

			public:
				inline ShaderSourceLocalTypeData(
					const String &name,
					const ParameterType type): m_name(name),
					m_type(type)
				{
				}

				inline ~ShaderSourceLocalTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline ParameterType get_type() const noexcept
				{
					return m_type;
				}

		};

		const ShaderSourceLocalTypeData
			shader_source_local_datas[] =
		{
			ShaderSourceLocalTypeData(
				String(UTF8("diffuse_colors_sum")), pt_vec3),
			ShaderSourceLocalTypeData(
				String(UTF8("specular_colors_sum")), pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8("shadow_values")),
				pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8("i")), pt_int),
			ShaderSourceLocalTypeData(String(UTF8("index")),
				pt_int),
			ShaderSourceLocalTypeData(String(
				UTF8("packed_light_indices")), pt_vec4),
			ShaderSourceLocalTypeData(String(UTF8("floor_values")),
				pt_vec4),
			ShaderSourceLocalTypeData(String(UTF8("unpack_const")),
				pt_vec4),
			ShaderSourceLocalTypeData(String(
				UTF8("packed_light_indices")), pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8("floor_values")),
				pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8("unpack_const")),
				pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8("light_indices")),
				pt_vec4),
			ShaderSourceLocalTypeData(String(UTF8("project_space")),
				pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8("light_index")),
				pt_float)
		};

		const Uint32 shader_source_local_datas_count =
			sizeof(shader_source_local_datas) /
			sizeof(ShaderSourceLocalTypeData);

		const String &get_str(const ShaderSourceLocalType local)
		{
			if (shader_source_local_datas_count > local)
			{
				return shader_source_local_datas[
					local].get_name();
			}

			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_source_local_datas_count
						 - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					local))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderSourceLocalType")));
		}

		ParameterType get_parameter_type(
			const ShaderSourceLocalType local)
		{
			if (shader_source_local_datas_count > local)
			{
				return shader_source_local_datas[
					local].get_type();
			}

			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_source_local_datas_count
						 - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					local))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderSourceLocalType")));
		}

		ShaderSourceParameter build_parameter(const String &source,
			const ShaderSourceLocalType local,
			const ParameterQualifierType qualifier)
		{
			if (shader_source_local_datas_count > local)
			{
				return ShaderSourceParameterBuilder::build(
					source,	shader_source_local_datas[
						local].get_name(),
					shader_source_local_datas[
						local].get_type(),
					qualifier);
			}

			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_source_local_datas_count
						 - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					local))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderSourceLocalType")));
		}

		void add_parameter(const String &source,
			const AutoParameterType auto_parameter,
			const ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &parameters,
			UniformBufferUsage &uniform_buffers)
		{
			ShaderSourceParameter tmp;

			tmp = ShaderSourceParameterBuilder::build(source,
				auto_parameter);

			ShaderSourceParameterBuilder::add_parameter(tmp, locals,
				parameters, uniform_buffers);
		}

		void add_parameter(const String &source,
			const SamplerParameterType sampler_parameter,
			const ParameterType type,
			const ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &parameters,
			UniformBufferUsage &uniform_buffers)
		{
			ShaderSourceParameter tmp;

			tmp = ShaderSourceParameterBuilder::build(source,
				sampler_parameter, type);

			ShaderSourceParameterBuilder::add_parameter(tmp, locals,
				parameters, uniform_buffers);
		}

		void add_parameter(const String &source,
			const CommonParameterType common_parameter,
			const ParameterQualifierType qualifier,
			const ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &parameters,
			UniformBufferUsage &uniform_buffers)
		{
			ShaderSourceParameter tmp;

			tmp = ShaderSourceParameterBuilder::build(source,
				common_parameter, qualifier);

			ShaderSourceParameterBuilder::add_parameter(tmp, locals,
				parameters, uniform_buffers);
		}

		void add_parameter(const String &source,
			const ShaderSourceLocalType local,
			const ParameterQualifierType qualifier,
			const ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &parameters,
			UniformBufferUsage &uniform_buffers)
		{
			ShaderSourceParameter tmp;

			tmp = build_parameter(source, local, qualifier);

			ShaderSourceParameterBuilder::add_parameter(tmp, locals,
				parameters, uniform_buffers);
		}

		void add_local(const String &source,
			const CommonParameterType common_parameter,
			const ParameterQualifierType qualifier,
			ShaderSourceParameterVector &locals,
			const ShaderSourceParameterVector &parameters,
			const UniformBufferUsage &uniform_buffers)
		{
			ShaderSourceParameter tmp;

			tmp = ShaderSourceParameterBuilder::build(source,
				common_parameter, qualifier);

			ShaderSourceParameterBuilder::add_local(tmp, locals,
				parameters, uniform_buffers);
		}

		void add_local(const String &source,
			const ShaderSourceLocalType local,
			const ParameterQualifierType qualifier,
			ShaderSourceParameterVector &locals,
			const ShaderSourceParameterVector &parameters,
			const UniformBufferUsage &uniform_buffers)
		{
			ShaderSourceParameter tmp;

			tmp = build_parameter(source, local, qualifier);

			ShaderSourceParameterBuilder::add_local(tmp, locals,
				parameters, uniform_buffers);
		}

		OutStream &operator<<(OutStream &str,
			const ShaderSourceLocalType value)
		{
			str << get_str(value);

			return str;
		}

		void make_parameter_local(const String &name,
			ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameterVector::iterator it;
			Uint32 i;

			i = 0;

			while (i < parameters.size())
			{
				it = parameters.begin() + i;

				if (it->get_name() == name)
				{
					locals.push_back(*it);
					parameters.erase(it);
					return;
				}

				++i;
			}
		}

		void add_parameters(
			const ShaderSourceParameterVector &function_parameters,
			const ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &globals,
			UniformBufferUsage &uniform_buffers)
		{
			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				function_parameters)
			{
				ShaderSourceParameterBuilder::add_parameter(
					parameter, locals, globals,
					uniform_buffers);
			}
		}

		void build_attributes(ShaderSourceParameterVector &inputs,
			ShaderSourceParameterVector &attributes)
		{
			ShaderSourceParameterVector::iterator it;
			VertexSemanticType vertex_semantic;
			Uint32 i;

			i = 0;

			while (i < inputs.size())
			{
				it = inputs.begin() + i;

				if (VertexElement::get_vertex_semantic(
					it->get_name(), vertex_semantic))
				{
					attributes.push_back(*it);
					inputs.erase(it);
				}
				else
				{
					++i;
				}
			}
		}

		void build_in_out(
			ShaderSourceParameterVector &in_parameters,
			ShaderSourceParameterVector &out_parameters,
			ShaderSourceParameterVector &in_out)
		{
			ShaderSourceParameterVector::iterator it;
			StringSet names, in_out_names;
			Uint32 i;

			i = 0;

			while (i < in_parameters.size())
			{
				it = in_parameters.begin() + i;

				if (it->get_qualifier() == pqt_in)
				{
					names.insert(it->get_name());
				}

				++i;
			}

			i = 0;

			while (i < out_parameters.size())
			{
				it = out_parameters.begin() + i;

				if ((it->get_qualifier() == pqt_out) &&
					(names.count(it->get_name()) > 0))
				{
					in_out_names.insert(it->get_name());
					in_out.push_back(*it);
					out_parameters.erase(it);
				}
				else
				{
					++i;
				}
			}

			i = 0;

			while (i < in_parameters.size())
			{
				it = in_parameters.begin() + i;

				if (in_out_names.count(it->get_name()) > 0)
				{
					in_parameters.erase(it);
				}
				else
				{
					++i;
				}
			}
		}

		void build_in_outs(const ShaderSourceParameterVector &varyings,
			ShaderSourceParameterVector &vertex_parameters,
			ShaderSourceParameterVector &geometry_parameters,
			ShaderSourceParameterVector &fragment_parameters,
			ShaderSourceParameterVector &geometry_in_parameters,
			ShaderSourceParameterVector &geometry_out_parameters)
		{
			ShaderSourceParameterVector::iterator it;
			StringSet names;
			Uint32 i, count;

			count = varyings.size();

			for (i = 0; i < count; ++i)
			{
				names.insert(varyings[i].get_name());
			}

			i = 0;

			while (i < geometry_parameters.size())
			{
				it = geometry_parameters.begin() + i;

				if (names.count(it->get_name()) > 0)
				{
					geometry_parameters.erase(it);
				}
				else
				{
					++i;
				}
			}

			build_in_out(geometry_parameters, vertex_parameters,
				geometry_in_parameters);

			build_in_out(fragment_parameters, geometry_parameters,
				geometry_out_parameters);
		}

		void write_parameter(const String &name_prefix,
			const ShaderSourceParameter &parameter,
			const ParameterSizeTypeUint16Map &array_sizes,
			const String &prefix, const String &suffix,
			OutStream &stream)
		{
			stream << prefix;
			parameter.write(name_prefix, array_sizes, stream);
			stream << suffix << UTF8("; /* ");
			stream << parameter.get_source() << UTF8(" */\n");
		}

		void write_parameters(const String &name_prefix,
			const ShaderSourceParameterVector &parameters,
			const ParameterSizeTypeUint16Map &array_sizes,
			const String &prefix, const String &suffix,
			OutStream &stream)
		{
			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				parameters)
			{
				write_parameter(name_prefix, parameter,
					array_sizes, prefix, suffix, stream);
			}
		}

		void write_parameters(const String &name_prefix,
			const ParameterQualifierType qualifier,
			const ShaderSourceParameterVector &parameters,
			const ParameterSizeTypeUint16Map &array_sizes,
			const String &prefix, const String &suffix,
			OutStream &stream)
		{
			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				parameters)
			{
				if (parameter.get_qualifier() == qualifier)
				{
					write_parameter(name_prefix, parameter,
						array_sizes, prefix, suffix,
						stream);
				}
			}
		}

		void write_parameters_assign_in(const String &name_prefix,
			const ShaderSourceParameterVector &parameters,
			const String &prefix, const bool use_block,
			OutStream &stream)
		{
			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				parameters)
			{
				stream << prefix << parameter.get_name();
				stream << UTF8(" = ") << name_prefix;

				if (use_block)
				{
					stream << UTF8(".");
				}

				stream << parameter.get_name();
				stream << UTF8("; /* ");
				stream << parameter.get_source();
				stream << UTF8(" */\n");
			}
		}

		void write_parameters_assign_out(const String &name_prefix,
			const ShaderSourceParameterVector &parameters,
			const String &prefix, const bool use_block,
			OutStream &stream)
		{
			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				parameters)
			{
				stream << prefix << name_prefix;

				if (use_block)
				{
					stream << UTF8(".");
				}

				stream << parameter.get_name() << UTF8(" = ");
				stream << parameter.get_name();
				stream << UTF8("; /* ");
				stream << parameter.get_source();
				stream << UTF8(" */\n");
			}
		}

		void write_parameter_indexed(const String &name_prefix,
			const ShaderSourceParameter &parameter,
			const String &prefix, const String &index,
			const bool use_block, OutStream &stream)
		{
			stream << prefix << name_prefix;

			if (use_block)
			{
				stream << UTF8("[") << index << UTF8("].");
			}

			stream << parameter.get_name();

			if (!use_block)
			{
				stream << UTF8("[") << index << UTF8("]");
			}
		}

		void write_parameter(const String &name_prefix,
			const ShaderSourceParameter &parameter,
			const String &prefix, const bool use_block,
			OutStream &stream)
		{
			stream << prefix << name_prefix;

			if (use_block)
			{
				stream << UTF8(".");
			}

			stream << parameter.get_name();
		}

		void write_function(const String &name,
			const String &function,
			const ShaderSourceParameterVector &locals,
			const ShaderSourceParameterVector &parameters, 
			const ParameterSizeTypeUint16Map &array_sizes,
			OutStream &str)
		{
			bool first;

			first = true;

			str << UTF8("void ") << name;
			str << UTF8("(");

			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				parameters)
			{
				parameter.write_parameter(String(),
					array_sizes, str, first);
			}

			str << UTF8(")\n");
			str << UTF8("{\n");
			write_parameters(String(), locals, array_sizes,
				String(UTF8("\t")), String(), str);
			str << UTF8("\n");
			str << function;
			str << UTF8("}\n\n");
		}

		void write_function_use(const String &name,
			const String &prefix,
			const ShaderSourceParameterVector &parameters, 
			OutStream &str)
		{
			bool first;

			first = true;

			str << prefix << name << UTF8("(");

			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				parameters)
			{
				parameter.write_name(String(), str, first);
			}

			str << UTF8(");\n");
		}

		void build_constants(
			const ParameterSizeTypeUint16Map &array_sizes,
			OutStream &str)
		{
			BOOST_FOREACH(const ParameterSizeTypeUint16Pair
				&parameter, array_sizes)
			{
				str << UTF8("const int ");
				str << parameter.first << UTF8(" = ");
				str << parameter.second << UTF8(";\n");
			}
		}

		class OptimizeShaderSource
		{
			private:
				glslopt_ctx* m_optimizer;
				glslopt_shader* m_shader;

			public:
				OptimizeShaderSource(glslopt_ctx* optimizer,
					glslopt_shader_type type,
					const String &shader_source);

				~OptimizeShaderSource() noexcept;

				String get_source();

		};

		OptimizeShaderSource::OptimizeShaderSource(
			glslopt_ctx* optimizer, glslopt_shader_type type,
			const String &source): m_optimizer(optimizer),
			m_shader(0)
		{
			m_shader = glslopt_optimize(m_optimizer, type,
				source.get().c_str(), 0);
		}

		OptimizeShaderSource::~OptimizeShaderSource() noexcept
		{
			glslopt_shader_delete(m_shader);
		}

		String OptimizeShaderSource::get_source()
		{
			if (glslopt_get_status(m_shader))
			{
				return String(glslopt_get_output(m_shader));
			}
			else
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(glslopt_get_log(
						m_shader)));
			}
		}

		String get_optimized_source(glslopt_ctx* optimizer,
			const String &prefix,
			const glslopt_shader_type type,
			const String &source)
		{
			OptimizeShaderSource oss(optimizer, type, source);
			std::string str;

			str = prefix;
			str += oss.get_source();

			return String(str);
		}

		ShaderVersionType get_shader_version_type(
			const OpenglVerionType opengl_version)
		{
			switch (opengl_version)
			{
				case ovt_2_1:
					return svt_120;
				case ovt_3_0:
					return svt_130;
				case ovt_3_1:
					return svt_140;
				case ovt_3_2:
					return svt_150;
				case ovt_3_3:
					return svt_330;
				case ovt_4_0:
					return svt_400;
				case ovt_4_1:
					return svt_410;
				case ovt_4_2:
					return svt_420;
			}

			return svt_120;
		}

		void write_defines(const String &prefix,
			const ShaderSourceParameterVector &parameters,
			OutStream &stream)
		{
			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				parameters)
			{
				stream << UTF8("#define ");
				stream << parameter.get_name();
				stream << UTF8(" ") << prefix;
				stream << parameter.get_name() << UTF8("\n");
			}
		}

		void write_attributes(const String &name_prefix,
			const ShaderSourceParameterVector &attributes,
			const ParameterSizeTypeUint16Map &array_sizes,
			OutStream &stream)
		{
			VertexSemanticType vertex_semantic;

			BOOST_FOREACH(const ShaderSourceParameter &attribute,
				attributes)
			{
				if (VertexElement::get_vertex_semantic(
					attribute.get_name(), vertex_semantic))
				{
					stream << UTF8("layout(location = ");
					stream << static_cast<Uint16>(
						vertex_semantic);
					stream << UTF8(") ");
					write_parameter(name_prefix, attribute,
						array_sizes,
						String(UTF8("in ")), String(),
						stream);
				}
			}
		}

		String get_output_type(const ShaderOutputType shader_output,
			const Uint16 count)
		{
			switch (shader_output)
			{
				case sot_int:
					switch (count)
					{
						case 1:
							return String(UTF8(
								"int"));
						case 2:
							return String(UTF8(
								"ivec2"));
						case 3:
							return String(UTF8(
								"ivec3"));
						case 4:
							return String(UTF8(
								"ivec4"));
					}
					break;
				case sot_uint:
					switch (count)
					{
						case 1:
							return String(UTF8(
								"unsigned "
									"int"));
						case 2:
							return String(UTF8(
								"uvec2"));
						case 3:
							return String(UTF8(
								"uvec3"));
						case 4:
							return String(UTF8(
								"uvec4"));
					}
					break;
				case sot_float:
				default:
					switch (count)
					{
						case 1:
							return String(UTF8(
								"float"));
						case 2:
							return String(UTF8(
								"vec2"));
						case 3:
							return String(UTF8(
								"vec3"));
						case 4:
							return String(UTF8(
								"vec4"));
					}
					break;
			}

			return String(UTF8("vec4"));
		}

		String get_output_swizzle(const Uint16 count)
		{
			switch (count)
			{
				case 1:
					return String(UTF8(".r"));
				case 2:
					return String(UTF8(".rg"));
				case 3:
					return String(UTF8(".rgb"));
				case 4:
					return String();
			}

			return String();
		}

	}

	class ShaderSourceBuilder::ShaderSourceOptimizer
	{
		private:
			glslopt_ctx* m_optimizer;

		public:
			inline ShaderSourceOptimizer()
			{
				m_optimizer = glslopt_initialize(false, false);
			}

			inline ~ShaderSourceOptimizer() noexcept
			{
				glslopt_cleanup(m_optimizer);
			}

			inline glslopt_ctx* get_optimizer() noexcept
			{
				return m_optimizer;
			}

	};

	enum ShaderSourceBuildOptionTypes
	{
		ssbot_normal = 0,
		ssbot_tangent,
		ssbot_transparent,
		ssbot_view_direction,
		ssbot_view_position,
		ssbot_tbn_matrix,
		ssbot_fog,
		ssbot_alpha_test,
		ssbot_alpha_write,
		ssbot_use_functions,
		ssbot_lighting,
		ssbot_light_indexed_deferred,
		ssbot_shadow_uv_ddx_ddy,
		ssbot_node_based_effect
	};

	class ShaderSourceBuilder::ShaderSourceBuildData
	{
		private:
			ShaderSourceSharedPrMap m_shader_sources;
			ShaderSourceTypeStringMap m_sources;
			ShaderVersionType m_version;
			ShaderBuildType m_shader_build;
			ShaderOutputType m_shader_output;
			std::set<ShaderSourceBuildOptionTypes> m_options;
			Uint8Array4 m_output_channels;
			Uint16 m_vertex_lights_count;
			Uint16 m_fragment_lights_count;
			Uint16 m_lights_count;
			Uint16 m_shadow_maps_count;
			Uint16 m_render_targets_count;

		public:
			inline ShaderSourceBuildData():	m_version(svt_120),
				m_shader_build(sbt_default),
				m_shader_output(sot_float),
				m_vertex_lights_count(0),
				m_fragment_lights_count(0), m_lights_count(0),
				m_shadow_maps_count(0),
				m_render_targets_count(0)
			{
				BOOST_FOREACH(Uint8 &output_channel,
					m_output_channels)
				{
					output_channel = 0;
				}
			}

			inline ShaderSourceBuildData(
				const ShaderSourceTypeStringMap &sources,
				const ShaderVersionType version,
				const ShaderBuildType shader_build,
				const ShaderOutputType shader_output,
				const Uint8Array4 &output_channels,
				const Uint16 vertex_lights_count,
				const Uint16 fragment_lights_count,
				const Uint16 lights_count,
				const Uint16 shadow_maps_count,
				const Uint16 render_targets_count):
				m_sources(sources), m_version(version),
				m_shader_build(shader_build),
				m_shader_output(shader_output),
				m_output_channels(output_channels),
				m_vertex_lights_count(vertex_lights_count),
				m_fragment_lights_count(fragment_lights_count),
				m_lights_count(lights_count),
				m_shadow_maps_count(shadow_maps_count),
				m_render_targets_count(render_targets_count)
			{
				Sint32 tmp;

				tmp = lights_count - fragment_lights_count;

				m_vertex_lights_count = std::min(
					vertex_lights_count,
					static_cast<Uint16>(std::max(0, tmp)));
				m_fragment_lights_count = std::min(lights_count,
					fragment_lights_count);
			}

			/**
			 * Default destructor.
			 */
			inline ~ShaderSourceBuildData() noexcept
			{
			}

			/**
			 * Returns the options the shader uses.
			 */
			inline void set_option(
				const ShaderSourceBuildOptionTypes option,
				const bool enabled) noexcept
			{
				if (enabled)
				{
					m_options.insert(option);
				}
				else
				{
					m_options.erase(option);
				}
			}

			/**
			 * Returns the sources of the different shader parts.
			 */
			inline const ShaderSourceTypeStringMap &get_sources()
				const noexcept
			{
				return m_sources;
			}

			/**
			 * Returns the shader build type.
			 */
			inline ShaderBuildType get_shader_build() const
				noexcept
			{
				return m_shader_build;
			}

			/**
			 * Returns the shader output type.
			 */
			inline ShaderOutputType get_shader_output() const
				noexcept
			{
				return m_shader_output;
			}

			inline ShaderVersionType get_version() const noexcept
			{
				return m_version;
			}

			/**
			 * Returns the options the shader uses.
			 */
			inline bool get_option(
				const ShaderSourceBuildOptionTypes option) const
				noexcept
			{
				return m_options.count(option);
			}

			/**
			 * Returns the number of lights the shader uses.
			 */
			inline Uint16 get_lights_count() const noexcept
			{
				return m_lights_count;
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
			 * Returns the number of shadow maps the shader uses.
			 */
			inline Uint16 get_shadow_maps_count() const noexcept
			{
				return m_shadow_maps_count;
			}

			/**
			 * Returns the number of render targets the shader uses.
			 */
			inline Uint16 get_render_targets_count() const noexcept
			{
				return m_render_targets_count;
			}

			inline Uint16 get_output_channel_count(
				const Uint16 render_target) const
			{
				RANGE_CECK_MAX(render_target,
					m_output_channels.size() - 1,
					UTF8("render target too big"));

				return m_output_channels[render_target];
			}

			bool get_shader_source(
				const ShaderSourceTypeStringPair &index,
				AbstractShaderSourceSharedPtr &shader_source)
			{
				ShaderSourceSharedPrMap::const_iterator found;

				found = m_shader_sources.find(index);

				if (found != m_shader_sources.end())
				{
					shader_source = found->second;

					return true;
				}

				return false;
			}

			void insert_shader_source(
				const ShaderSourceTypeStringPair &index,
				AbstractShaderSourceSharedPtr &shader_source)
			{
				m_shader_sources[index] = shader_source;
			}

	};

	ShaderSourceBuilder::ShaderSourceBuilder(
		const GlobalVarsConstSharedPtr &global_vars,
		const FileSystemConstSharedPtr &file_system,
		const UniformBufferDescriptionCacheWeakPtr
			&uniform_buffer_description_cache):
		m_global_vars(global_vars), m_file_system(file_system),
		m_uniform_buffer_description_cache(
			uniform_buffer_description_cache)
	{
		assert(m_global_vars.get() != nullptr);
		assert(m_file_system.get() != nullptr);
		assert(!m_uniform_buffer_description_cache.expired());

		m_optimizer.reset(new ShaderSourceOptimizer());

		m_shadow_scale = 0.8f;
		m_vertex_lights_count = 4;
		m_fragment_lights_count = 4;
		m_bones_count = 72;
		m_dynamic_lights_count = false;
	}

	ShaderSourceBuilder::~ShaderSourceBuilder() noexcept
	{
	}

	void ShaderSourceBuilder::build_decode_normal(const String &indent,
		OutStream &functions) const
	{
		functions << indent << UTF8("/**") << std::endl;
		functions << indent << UTF8(" * Converts from Lambert ");
		functions << UTF8("Azimuthal Equal-Area") << std::endl;
		functions << indent << UTF8(" * projection. Optimized for ");
		functions << UTF8("normalized Uint8 values.") << std::endl;
		functions << indent << UTF8(" * @param value The Lambert ");
		functions << UTF8("Azimuthal Equal-Area") << std::endl;
		functions << indent << UTF8(" * projection vector to use ");
		functions << UTF8("(0..1).") << std::endl;
		functions << indent << UTF8(" * @return The normalized ");
		functions << UTF8("vector.") << std::endl;
		functions << indent << UTF8(" */") << std::endl;

		functions << indent << UTF8("vec3 decode_normal(const in ");
		functions << UTF8("vec2 normal)") << std::endl;
		functions << indent << UTF8("{") << std::endl;
		functions << indent << UTF8("\tvec2 fenc;") << std::endl;
		functions << indent << UTF8("\tfloat f, g;") << std::endl;
		functions << std::endl;
		functions << indent << UTF8("\tfenc = normal * vec2(");
		functions << UTF8("3.968871595) - vec2(1.984435798);");
		functions << std::endl;
		functions << indent << UTF8("\tf = dot(fenc, fenc);");
		functions << std::endl;
		functions << indent << UTF8("\tg = sqrt(1.0 - f / 4.0);");
		functions << std::endl;
		functions << indent << UTF8("\treturn vec3(fenc * g, 1.0 -");
		functions << UTF8(" f / 2.0);") << std::endl;
		functions << indent << UTF8("}") << std::endl;
		functions << std::endl;
	}

	void ShaderSourceBuilder::build_decode_terrain_displacement(
		const String &indent, OutStream &functions) const
	{
		functions << indent << UTF8("/**") << std::endl;
		functions << indent << UTF8(" * Convertes the normalized ");
		functions << UTF8("rgb10_a2 vector to the") << std::endl;
		functions << indent << UTF8(" * scaled terrain ");
		functions << UTF8("displacement vector.") << std::endl;
		functions << indent << UTF8(" * @param value The rgb10_a2 ");
		functions << UTF8("normalized value (0..1).") << std::endl;
		functions << indent << UTF8(" * @return The terrain ");
		functions << UTF8("displacement vector.") << std::endl;
		functions << indent << UTF8(" */") << std::endl;

		functions << indent << UTF8("vec3 decode_terrain_displacement");
		functions << UTF8("(const in vec4 vector)") << std::endl;
		functions << indent << UTF8("{") << std::endl;
		functions << indent << UTF8("\tvec3 result, tmp;") << std::endl;
		functions << std::endl;
		functions << indent << UTF8("\tresult = vector.xyz;");
		functions << std::endl;
		functions << indent << UTF8("\tresult.xy = result.xy * 2.0 -");
		functions << UTF8(" 1.0;") << std::endl;
		functions << indent << UTF8("\tresult *= vec3(");
		functions << AbstractTerrain::get_vector_scale().x;
		functions << UTF8(",");
		functions << AbstractTerrain::get_vector_scale().y;
		functions << UTF8(",");
		functions << AbstractTerrain::get_vector_scale().z;
		functions << UTF8(");") << std::endl;
		functions << std::endl;
		functions << indent << UTF8("\treturn result;") << std::endl;
		functions << indent << UTF8("}") << std::endl;
		functions << std::endl;
	}

	bool ShaderSourceBuilder::get_shader_source(
		const ShaderSourceType shader_source_type,
		ShaderSourceBuildData &build_data,
		AbstractShaderSourceSharedPtr &shader_source) const
	{
		ShaderSourceSharedPrMap::const_iterator found;
		ShaderSourceTypeStringMap::const_iterator index;

		index = build_data.get_sources().find(shader_source_type);

		if (index == build_data.get_sources().end())
		{
			shader_source.reset();

			return false;
		}

		found = m_shader_sources.find(*index);

		if (found != m_shader_sources.end())
		{
			shader_source = found->second;

			return true;
		}

		if (build_data.get_shader_source(*index, shader_source))
		{
			return true;
		}

		load_shader_source(index->second, shader_source);

		if (shader_source.get() != nullptr)
		{
			build_data.insert_shader_source(*index, shader_source);

			return true;
		}

		shader_source.reset();

		LOG_ERROR(lt_shader_source, UTF8("Shader source type not found"
			" %1%-%2%"), index->first % index->second);

		return false;
	}

	void ShaderSourceBuilder::load_shader_source(const String &file_name,
		AbstractShaderSourceSharedPtr &shader_source) const
	{
		XmlReaderSharedPtr xml_reader;
		ShaderSourceTypeStringPair index;
		String name;

		shader_source.reset();

		try
		{
			if (!XmlReader::get_xml_reader(get_file_system(),
				file_name, xml_reader))
			{
				return;
			}

			assert(xml_reader.get() != nullptr);

			name = String(std::string(reinterpret_cast<const char*>(
				xml_reader->get_root_node()->name)));

			if (name == ShaderSourceTerrain::get_xml_id())
			{
				shader_source =	boost::make_shared<
					ShaderSourceTerrain>();
			}
			else
			{
				if (name == EffectNodes::get_xml_id())
				{
					shader_source = boost::make_shared<
						EffectNodes>();
				}
				else
				{
					if (name == ShaderSource::get_xml_id())
					{
						shader_source =
							boost::make_shared<
								ShaderSource>();
					}
					else
					{
						LOG_ERROR(lt_shader_source,
							UTF8("Shader source "
								"'%1%' has "
								"invalid id "
								"'%2%'"),
								file_name %
								name);
						return;
					}
				}
			}

			shader_source->load_xml(xml_reader->get_root_node());
		}
		catch (boost::exception &exception)
		{
			shader_source.reset();

			exception << boost::errinfo_file_name(file_name);

			LOG_EXCEPTION(exception);
		}
		catch (std::exception &exception)
		{
			shader_source.reset();

			LOG_EXCEPTION(exception);
		}
	}

	bool ShaderSourceBuilder::load_shader_source(const String &file_name)
	{
		AbstractShaderSourceSharedPtr shader_source;
		ShaderSourceTypeStringPair index;
		std::pair<ShaderSourceTypeStringPair, ShaderSourceSharedPtr>
			data;

		try
		{
			load_shader_source(file_name, shader_source);

			if (shader_source.get() == nullptr)
			{
				return false;
			}

			index.first = shader_source->get_type();
			index.second = shader_source->get_name();

			m_shader_sources[index] = shader_source;

			LOG_DEBUG(lt_shader_source, UTF8("Shader source type "
				"%1%-%2% loaded from file '%3%'"), index.first
				% index.second % file_name);

			return true;
		}
		catch (boost::exception &exception)
		{
			exception << boost::errinfo_file_name(file_name);

			LOG_EXCEPTION(exception);
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}

		return false;
	}

	void ShaderSourceBuilder::load_sources(const xmlNodePtr node)
	{
		xmlNodePtr it;
		Uint32 i, count;
		ShaderSourceType type;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("sources")) != 0)
		{
			return;
		}

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		count = ShaderSourceUtil::get_shader_source_count();

		do
		{
			for (i = 0; i < count; ++i)
			{
				type = static_cast<ShaderSourceType>(i);

				if (xmlStrcmp(it->name, BAD_CAST
					ShaderSourceUtil::get_str(type).get(
						).c_str()) == 0)
				{
					m_default_sources[type] =
						XmlUtil::get_string_value(it);

					break;
				}
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceBuilder::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;
		String name, file_name;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("shaders")) != 0)
		{
			return;
		}

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		m_shadow_scale = 0.8f;
		m_vertex_lights_count = 4;
		m_fragment_lights_count = 4;
		m_bones_count = 72;
		m_dynamic_lights_count = false;

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("shadow_scale"))
				== 0)
			{
				m_shadow_scale = XmlUtil::get_float_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("vertex_lights_count")) == 0)
			{
				m_vertex_lights_count =
					XmlUtil::get_u16_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("fragment_lights_count")) == 0)
			{
				m_fragment_lights_count =
					XmlUtil::get_u16_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("bones_count"))
				== 0)
			{
				m_bones_count = XmlUtil::get_u16_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("dynamic_lights_count")) == 0)
			{
				m_dynamic_lights_count =
					XmlUtil::get_bool_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("sources"))
				== 0)
			{
				load_sources(it);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceBuilder::load_xml(const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = boost::make_shared<XmlReader>(get_file_system(),
			file_name);

		load_xml(xml_reader->get_root_node());
	}

	void ShaderSourceBuilder::load_shader_sources(const String &dir)
	{
		StringSet files;

		files = get_file_system()->get_files(dir,
			String(UTF8("*.xml")));

		BOOST_FOREACH(const String &file, files)
		{
			load_shader_source(file);
		}
	}

	bool ShaderSourceBuilder::build_function(
		const ParameterSizeTypeUint16Map &array_sizes,
		const ShaderSourceParameterVector &locals,
		const String &indent, const ShaderSourceType shader_source_type,
		const ShaderType shader_type, ShaderSourceBuildData &build_data,
		OutStream &stream, OutStream &functions,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		AbstractShaderSourceSharedPtr shader_source;

		if (!get_shader_source(shader_source_type, build_data,
			shader_source))
		{
			return false;
		}

		if (build_data.get_option(ssbot_use_functions))
		{
			shader_source->build_function(locals, array_sizes,
				String(UTF8("")), String(UTF8("")), indent,
				shader_type, build_data.get_version(), stream,
				functions, globals, uniform_buffers);
		}
		else
		{
			shader_source->build_source(locals, indent,
				shader_type, build_data.get_version(), stream,
				globals, uniform_buffers);
		}

		return true;
	}

	void ShaderSourceBuilder::build_light_index_x4_lights(
		const ShaderSourceBuildData &build_data,
		const String &indent, const String &loop_indent,
		OutStream &stream) const
	{
		/**
		 * Look up the bit planes texture
		 */
		stream << indent << UTF8("/* Look up the bit ");
		stream << UTF8("planes texture */\n");

		stream << indent << sslt_packed_light_indices_4;

		if (build_data.get_version() >= svt_130)
		{
			stream << UTF8(" = texelFetch(") << spt_effect_14;
			stream << UTF8(", ivec2(gl_FragCoord.xy), 0);\n");
		}
		else
		{
			stream << UTF8(" = texture2DProj(");
			stream << spt_effect_14 << UTF8(", ");
			stream << sslt_project_space << UTF8(");\n");
		}

		if (build_data.get_shader_build() ==
			sbt_debug_packed_light_index)
		{
			stream << loop_indent;
			stream << sslt_light_indices << UTF8(" = ");
			stream << sslt_packed_light_indices_4;
			stream << UTF8(";\n");
		}

		/**
		 * Unpack each lighting channel
		 */
		stream << indent << UTF8("/* Unpack each ");
		stream << UTF8("lighting channel */\n");

		stream << indent << sslt_unpack_const_4;

		stream << UTF8("= vec4(4.0, 16.0, 64.0, 256.0)");

		if (build_data.get_version() < svt_130)
		{
			stream << UTF8(" / 256.0");
		}

		stream << UTF8(";\n");

		/**
		 * Expand the packed light values to the 0 .. 255 range
		 */
		stream << indent << UTF8("/* Expand the packed");
		stream << UTF8(" light values to the 0 .. 255 range");
		stream << UTF8("  */\n");

		stream << indent << sslt_floor_values_4;
		stream << UTF8(" = ceil(");
		stream << sslt_packed_light_indices_4;
		stream << UTF8(" * 254.5);\n");

		stream << indent << UTF8("for (") << sslt_i;
		stream << UTF8(" = 0; ") << sslt_i;
		stream << UTF8(" < 4; ++i)\n");
		stream << indent << UTF8("{\n");

		/**
		 * Exit loop if no more lights
		 */
		stream << loop_indent << UTF8("/* Exit loop if no more ");
		stream << UTF8("lights */\n");
		stream << loop_indent << UTF8("if (all(lessThanEqual(");
		stream << sslt_floor_values_4;
		stream << UTF8(", vec4(0))))\n");
		stream << loop_indent << UTF8("{\n");
		stream << loop_indent << UTF8("\tbreak;\n");
		stream << loop_indent << UTF8("}\n");

		stream << loop_indent;
		stream << UTF8("/* Shift two bits down */\n");
		stream << loop_indent;
		stream << sslt_packed_light_indices_4;
		stream << UTF8(" = ") << sslt_floor_values_4;
		stream << UTF8(" * 0.25;\n");

		stream << loop_indent;
		stream << UTF8("/* Remove shifted bits */\n");
		stream << loop_indent << sslt_floor_values_4;
		stream << UTF8(" = floor(");
		stream << sslt_packed_light_indices_4;
		stream << UTF8(");\n");

		if (build_data.get_version() < svt_130)
		{
			stream << loop_indent << sslt_light_index;
			stream << UTF8(" = ");
		}
		else
		{
			stream << loop_indent << sslt_index;
			stream << UTF8(" = int(");
		}

		stream << UTF8("dot(") << sslt_packed_light_indices_4;
		stream << UTF8(" - ") << sslt_floor_values_4;
		stream << UTF8(", ") << sslt_unpack_const_4;
		stream << UTF8(")");

		if (build_data.get_version() >= svt_130)
		{
			stream << UTF8(" + 0.5)");
		}

		stream << UTF8(";\n");
	}

	void ShaderSourceBuilder::build_light_index_x5_lights(
		const ShaderSourceBuildData &build_data,
		const String &indent, const String &loop_indent,
		OutStream &stream) const
	{
		/**
		 * Look up the bit planes texture
		 */
		stream << indent << UTF8("/* Look up the bit ");
		stream << UTF8("planes texture */\n");

		stream << indent << sslt_packed_light_indices_3;

		if (build_data.get_version() >= svt_130)
		{
			stream << UTF8(" = texelFetch(") << spt_effect_14;
			stream << UTF8(", ivec2(gl_FragCoord.xy), 0).rgb;\n");
		}
		else
		{
			stream << UTF8(" = texture2DProj(");
			stream << spt_effect_14 << UTF8(", ");
			stream << sslt_project_space << UTF8(").rgb;\n");
		}

		if (build_data.get_shader_build() ==
			sbt_debug_packed_light_index)
		{
			stream << loop_indent;
			stream << sslt_light_indices;
			stream << UTF8(" = vec4(");
			stream << sslt_packed_light_indices_3;
			stream << UTF8(", 0.0);\n");
		}

		/**
		 * Unpack each lighting channel
		 */
		stream << indent << UTF8("/* Unpack each lighting channel ");
		stream << UTF8("*/\n");

		stream << indent << sslt_unpack_const_3;

		stream << UTF8("= vec3(4.0, 16.0, 64.0)");

		if (build_data.get_version() < svt_130)
		{
			stream << UTF8(" / 64.0");
		}

		stream << UTF8(";\n");

		/**
		 * Expand the packed light values to the 0 .. 1023 range
		 */
		stream << indent << UTF8("/* Expand the packed light values ");
		stream << UTF8("to the 0 .. 1023 range */\n");

		stream << indent << sslt_floor_values_3 << UTF8(" = ceil(");
		stream << sslt_packed_light_indices_3 << UTF8(" * 1022.5);\n");

		stream << indent << UTF8("for (") << sslt_i << UTF8(" = 0; ");
		stream << sslt_i << UTF8(" < 5; ++i)\n");
		stream << indent << UTF8("{\n");

		/**
		 * Exit loop if no more lights
		 */
		stream << loop_indent << UTF8("/* Exit loop if no more ");
		stream << UTF8("lights */\n");
		stream << loop_indent << UTF8("if (all(lessThanEqual(");
		stream << sslt_floor_values_3;
		stream << UTF8(", vec3(0))))\n");
		stream << loop_indent << UTF8("{\n");
		stream << loop_indent << UTF8("\tbreak;\n");
		stream << loop_indent << UTF8("}\n");

		stream << loop_indent;
		stream << UTF8("/* Shift two bits down */\n");
		stream << loop_indent;
		stream << sslt_packed_light_indices_3 << UTF8(" = ");
		stream << sslt_floor_values_3 << UTF8(" * 0.25;\n");

		stream << loop_indent;
		stream << UTF8("/* Remove shifted bits */\n");
		stream << loop_indent << sslt_floor_values_3;
		stream << UTF8(" = floor(");
		stream << sslt_packed_light_indices_3 << UTF8(");\n");

		if (build_data.get_version() < svt_130)
		{
			stream << loop_indent << sslt_light_index;
			stream << UTF8(" = ");
		}
		else
		{
			stream << loop_indent << sslt_index;
			stream << UTF8(" = int(");
		}

		stream << UTF8("dot(") << sslt_packed_light_indices_3;
		stream << UTF8(" - ") << sslt_floor_values_3;
		stream << UTF8(", ") << sslt_unpack_const_3;
		stream << UTF8(")");

		if (build_data.get_version() >= svt_130)
		{
			stream << UTF8(" + 0.5)");
		}

		stream << UTF8(";\n");
	}

	void ShaderSourceBuilder::build_light_index_x8_lights(
		const ShaderSourceBuildData &build_data,
		const String &indent, const String &loop_indent,
		OutStream &stream) const
	{
		/**
		 * Look up the bit planes texture
		 */
		stream << indent << UTF8("/* Look up the bit ");
		stream << UTF8("planes texture */\n");

		stream << indent << sslt_packed_light_indices_4;

		if (build_data.get_version() >= svt_130)
		{
			stream << UTF8(" = texelFetch(");
			stream << spt_effect_14;
			stream << UTF8(", ivec2(gl_FragCoord.xy), 0)");
			stream << UTF8(";\n");
		}
		else
		{
			stream << UTF8(" = texture2DProj(");
			stream << spt_effect_14 << UTF8(", ");
			stream << sslt_project_space << UTF8(");\n");
		}

		if (build_data.get_shader_build() ==
			sbt_debug_packed_light_index)
		{
			stream << loop_indent;
			stream << sslt_light_indices << UTF8(" = ");
			stream << sslt_packed_light_indices_4;
			stream << UTF8(";\n");
		}

		/**
		 * Unpack each lighting channel
		 */
		stream << indent << UTF8("/* Unpack each ");
		stream << UTF8("lighting channel */\n");

		stream << indent << sslt_unpack_const_4;

		stream << UTF8("= vec4(4.0, 16.0, 64.0, 256.0)");

		if (build_data.get_version() < svt_130)
		{
			stream << UTF8(" / 256.0");
		}

		stream << UTF8(";\n");

		/**
		 * Expand the packed light values to the 0 .. 255 range
		 */
		stream << indent << UTF8("/* Expand the packed");
		stream << UTF8(" light values to the 0 .. 255 range");
		stream << UTF8("  */\n");

		stream << indent << sslt_floor_values_4;
		stream << UTF8(" = ceil(");
		stream << sslt_packed_light_indices_4;
		stream << UTF8(" * 65534.5);\n");

		stream << indent << UTF8("for (") << sslt_i;
		stream << UTF8(" = 0; ") << sslt_i;
		stream << UTF8(" < 8; ++i)\n");
		stream << indent << UTF8("{\n");

		/**
		 * Exit loop if no more lights
		 */
		stream << loop_indent << UTF8("/* Exit loop if no more ");
		stream << UTF8("lights */\n");
		stream << loop_indent << UTF8("if (all(lessThanEqual(");
		stream << sslt_floor_values_4;
		stream << UTF8(", vec4(0))))\n");
		stream << loop_indent << UTF8("{\n");
		stream << loop_indent << UTF8("\tbreak;\n");
		stream << loop_indent << UTF8("}\n");

		stream << loop_indent;
		stream << UTF8("/* Shift two bits down */\n");
		stream << loop_indent;
		stream << sslt_packed_light_indices_4;
		stream << UTF8(" = ") << sslt_floor_values_4;
		stream << UTF8(" * 0.25;\n");

		stream << loop_indent;
		stream << UTF8("/* Remove shifted bits */\n");
		stream << loop_indent << sslt_floor_values_4;
		stream << UTF8(" = floor(");
		stream << sslt_packed_light_indices_4;
		stream << UTF8(");\n");

		if (build_data.get_version() < svt_130)
		{
			stream << loop_indent << sslt_light_index;
			stream << UTF8(" = ");
		}
		else
		{
			stream << loop_indent << sslt_index;
			stream << UTF8(" = int(");
		}

		stream << UTF8("dot(") << sslt_packed_light_indices_4;
		stream << UTF8(" - ") << sslt_floor_values_4;
		stream << UTF8(", ") << sslt_unpack_const_4;
		stream << UTF8(")");

		if (build_data.get_version() >= svt_130)
		{
			stream << UTF8(" + 0.5)");
		}

		stream << UTF8(";\n");
	}

	void ShaderSourceBuilder::build_light_index_lights(
		const ParameterSizeTypeUint16Map &array_sizes,
		const ShaderSourceParameterVector &locals, 
		const String &indent, const bool shadow,
		ShaderSourceBuildData &build_data, OutStream &main,
		OutStream &functions, ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		ShaderSourceParameterVector function_locals, bak;
		ShaderSourceParameterVector function_parameters;
		StringStream stream;
		String local_indent, local_loop_indent;
		String lighting;

		lighting = UTF8("lighting");
		local_indent = indent.get() + UTF8("\t");
		local_loop_indent = local_indent.get() + UTF8("\t");

		add_local(String(UTF8("lighting")), sslt_i, pqt_out,
			function_locals, function_parameters,
			uniform_buffers);
		add_local(String(UTF8("lighting")), sslt_shadow_values,
			pqt_out, function_locals, function_parameters,
			uniform_buffers);

		if (get_global_vars()->get_light_system() == lst_lidr_x5)
		{
			add_local(String(UTF8("lighting")),
				sslt_packed_light_indices_3, pqt_out,
				function_locals, function_parameters,
				uniform_buffers);
			add_local(String(UTF8("lighting")), sslt_floor_values_3,
				pqt_out, function_locals, function_parameters,
				uniform_buffers);
			add_local(String(UTF8("lighting")), sslt_unpack_const_3,
				pqt_out,function_locals, function_parameters,
				uniform_buffers);
		}
		else
		{
			add_local(String(UTF8("lighting")),
				sslt_packed_light_indices_4, pqt_out,
				function_locals, function_parameters,
				uniform_buffers);
			add_local(String(UTF8("lighting")), sslt_floor_values_4,
				pqt_out, function_locals, function_parameters,
				uniform_buffers);
			add_local(String(UTF8("lighting")), sslt_unpack_const_4,
				pqt_out,function_locals, function_parameters,
				uniform_buffers);
		}

		add_parameter(String(UTF8("lighting")), spt_effect_12,
			pt_sampler1D, function_locals, function_parameters,
			uniform_buffers);
		add_parameter(String(UTF8("lighting")), spt_effect_13,
			pt_sampler1D, function_locals, function_parameters,
			uniform_buffers);
		add_parameter(String(UTF8("lighting")), spt_effect_14,
			pt_sampler2D, function_locals, function_parameters,
			uniform_buffers);

		stream << UTF8("\n");

		if (build_data.get_shader_build() == sbt_debug_diffuse_light)
		{
			add_parameter(String(UTF8("lighting")),
				sslt_diffuse_colors_sum, pqt_out,
				function_locals, function_parameters,
				uniform_buffers);
		}
		else
		{
			add_local(String(UTF8("lighting")),
				sslt_diffuse_colors_sum, pqt_out,
				function_locals, function_parameters,
				uniform_buffers);
		}

		if ((build_data.get_shader_build() ==
			sbt_debug_packed_light_index) ||
			(build_data.get_shader_build() ==
				sbt_debug_light_index))
		{
			add_parameter(String(UTF8("lighting")),
				sslt_light_indices, pqt_out,
				function_locals, function_parameters,
				uniform_buffers);

			stream << local_indent << sslt_light_indices;
			stream << UTF8(" = vec4(0.0);\n");
		}

		stream << local_indent << sslt_diffuse_colors_sum;
		stream << UTF8(" = ");

		add_parameter(String(UTF8("lighting")),
			apt_sky_ground_hemispheres, function_locals,
			function_parameters, uniform_buffers);

		stream << apt_sky_ground_hemispheres << UTF8("[0].rgb + ");
		stream << apt_sky_ground_hemispheres << UTF8("[1].rgb * ");
		stream << UTF8("(0.5 * ") << cpt_fragment_normal;
		stream << UTF8(".z + 0.5);\n");

		add_parameter(String(UTF8("lighting")), cpt_emission, pqt_in,
			function_locals, function_parameters, uniform_buffers);

		if (build_data.get_shader_build() == sbt_debug_specular_light)
		{
			add_parameter(String(UTF8("lighting")),
				sslt_specular_colors_sum, pqt_out,
				function_locals, function_parameters,
				uniform_buffers);
		}
		else
		{
			add_local(String(UTF8("lighting")),
				sslt_specular_colors_sum, pqt_out,
				function_locals, function_parameters,
				uniform_buffers);
		}

		stream << local_indent << sslt_diffuse_colors_sum;
		stream << UTF8(" += ") << cpt_emission << UTF8(";\n");

		add_parameter(String(UTF8("lighting")), cpt_specular, pqt_in,
			function_locals, function_parameters, uniform_buffers);
		add_parameter(String(UTF8("lighting")), cpt_gloss, pqt_in,
			function_locals, function_parameters, uniform_buffers);
		add_parameter(String(UTF8("lighting")), cpt_fragment_normal,
			pqt_in, function_locals, function_parameters,
			uniform_buffers);
		add_parameter(String(UTF8("lighting")),
			cpt_world_view_direction, pqt_in, function_locals,
			function_parameters, uniform_buffers);
		add_parameter(String(UTF8("lighting")),
			apt_sky_ground_hemispheres, function_locals,
			function_parameters, uniform_buffers);

		stream << local_indent << sslt_specular_colors_sum;
		stream << UTF8(" = mix(") << cpt_specular << UTF8(", min(");
		stream << UTF8("60.0 * ") << cpt_specular << UTF8(", 1.0), ");
		stream << UTF8("pow(1.0 - ") << cpt_gloss << UTF8(" * ");
		stream << UTF8("max(0.0, dot(") << cpt_fragment_normal;
		stream << UTF8(", -") << cpt_world_view_direction;
		stream << UTF8(".xyz)), 4.0)) * (");
		stream << apt_sky_ground_hemispheres << UTF8("[0].a + ");
		stream << apt_sky_ground_hemispheres << UTF8("[1].a * ");
		stream << UTF8("clamp(reflect(") << cpt_world_view_direction;
		stream << UTF8(".xyz, ") << cpt_fragment_normal << UTF8(").z");
		stream << UTF8(" / (1.0 - ") << cpt_gloss << UTF8(") * 0.5 + ");
		stream << UTF8("0.5, 0.0, 1.0));\n");

		if (shadow)
		{
			add_parameter(String(UTF8("lighting")), cpt_shadow,
				pqt_in, function_locals, function_parameters,
				uniform_buffers);

			stream << local_indent << sslt_shadow_values;
			stream << UTF8(" = vec3(") << cpt_shadow;
			stream << UTF8(" * ") << get_shadow_scale();
			stream << UTF8(" + ") << (1.0 - get_shadow_scale());
			stream << UTF8(", ") << cpt_shadow;
			stream << UTF8(", 1.0);\n");
		}

		stream << UTF8("\n");

		if (build_data.get_version() < svt_130)
		{
			add_parameter(String(UTF8("lighting")),
				sslt_project_space, pqt_in, function_locals,
				function_parameters, uniform_buffers);
			add_local(String(UTF8("lighting")), sslt_light_index,
				pqt_out, function_locals, function_parameters,
				uniform_buffers);
		}
		else
		{
			add_local(String(UTF8("lighting")), sslt_index,
				pqt_out, function_locals, function_parameters,
				uniform_buffers);
		}

		switch (get_global_vars()->get_light_system())
		{
			case lst_default:
				return;
			case lst_lidr_x4:
				build_light_index_x4_lights(build_data,
					local_indent, local_loop_indent,
					stream);
				break;
			case lst_lidr_x5:
				build_light_index_x5_lights(build_data,
					local_indent, local_loop_indent,
					stream);
				break;
			case lst_lidr_x8:
				build_light_index_x8_lights(build_data,
					local_indent, local_loop_indent,
					stream);
				break;
		};

		if (build_data.get_shader_build() == sbt_debug_light_index)
		{
			stream << local_loop_indent << sslt_light_indices;
			stream << UTF8(".gba = ") << sslt_light_indices;
			stream << UTF8(".rgb;\n");
			stream << local_loop_indent << sslt_light_indices;
			stream << UTF8(".r = ") << sslt_index;
			stream << UTF8(" / 510 + 0.5;\n");
		}

		add_local(String(UTF8("lighting")), cpt_light_color, pqt_in,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << cpt_light_color;

		if (build_data.get_version() >= svt_130)
		{
			stream << UTF8(" = texelFetch(") << spt_effect_13;
			stream << UTF8(", ") << sslt_index << UTF8(", 0);\n");
		}
		else
		{
			stream << UTF8(" = texture1D(") << spt_effect_13;
			stream << UTF8(", ") << sslt_light_index;
			stream << UTF8(");\n");
		}

		add_local(String(UTF8("lighting")), cpt_light_position, pqt_in,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << cpt_light_position;

		if (build_data.get_version() >= svt_130)
		{
			stream << UTF8(" = texelFetch(") << spt_effect_12;
			stream << UTF8(", ") << sslt_index << UTF8(", 0);\n");
		}
		else
		{
			stream << UTF8(" = texture1D(") << spt_effect_12;
			stream << UTF8(", ") << sslt_light_index;
			stream << UTF8(");\n");
		}

		build_function(array_sizes, function_locals, local_loop_indent,
			sst_light, st_fragment, build_data, stream,
			functions, function_parameters, uniform_buffers);

		bak = function_locals;

		make_parameter_local(
			CommonParameterUtil::get_str(cpt_diffuse_color),
			function_locals, function_parameters);
		make_parameter_local(
			CommonParameterUtil::get_str(cpt_specular_color),
			function_locals, function_parameters);

		add_local(String(UTF8("lighting")), cpt_diffuse_color, pqt_in,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << sslt_diffuse_colors_sum;
		stream << UTF8(" += ") << cpt_diffuse_color << UTF8(";\n");

		add_local(String(UTF8("lighting")), cpt_specular_color, pqt_in,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << sslt_specular_colors_sum;
		stream << UTF8(" += ") << cpt_specular_color << UTF8(";\n");

		stream << local_indent << UTF8("}\n\n");

		stream << local_indent << UTF8("for (") << sslt_i;
		stream << UTF8(" = 0") << UTF8("; ") << sslt_i << UTF8(" < ");

		if (get_dynamic_lights_count())
		{
			add_parameter(String(UTF8("lighting")),
				apt_dynamic_lights_count, function_locals,
				function_parameters, uniform_buffers);

			stream << apt_dynamic_lights_count;
		}
		else
		{
			stream << build_data.get_fragment_lights_count();
		}

		stream << UTF8("; ++") << sslt_i << UTF8(")\n");
		stream << local_indent << UTF8("{\n");

		add_local(String(UTF8("lighting")), cpt_light_color, pqt_in,
			function_locals, function_parameters, uniform_buffers);
		add_parameter(String(UTF8("lighting")), apt_light_colors,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << cpt_light_color;
		stream << UTF8(" = ") << apt_light_colors;
		stream << UTF8("[") << sslt_i << UTF8("];\n");

		add_local(String(UTF8("lighting")), cpt_light_position, pqt_in,
			function_locals, function_parameters, uniform_buffers);
		add_parameter(String(UTF8("lighting")), apt_light_positions,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << cpt_light_position;
		stream << UTF8(" = ") << apt_light_positions;
		stream << UTF8("[") << sslt_i << UTF8("];\n");

		build_function(array_sizes, bak, local_loop_indent, sst_light,
			st_fragment, build_data, stream, functions,
			function_parameters, uniform_buffers);

		add_local(String(UTF8("lighting")), cpt_diffuse_color, pqt_in,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << sslt_diffuse_colors_sum;
		stream << UTF8(" += ") << cpt_diffuse_color;

		if (shadow)
		{
			add_parameter(String(UTF8("lighting")), cpt_shadow,
				pqt_in, function_locals, function_parameters,
				uniform_buffers);

			stream << UTF8(" * ") << sslt_shadow_values;
			stream << UTF8(".x");
		}

		stream << UTF8(";\n");

		add_local(String(UTF8("lighting")), cpt_specular_color, pqt_in,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << sslt_specular_colors_sum;
		stream << UTF8(" += ") << cpt_specular_color;

		if (shadow)
		{
			stream << UTF8(" * ") << sslt_shadow_values;
			stream << UTF8(".y");
		}

		stream << UTF8(";\n");

		if (shadow)
		{
			stream << local_loop_indent << sslt_shadow_values;
			stream << UTF8(".xy = ") << sslt_shadow_values;
			stream << UTF8(".zz;\n");
		}

		stream << local_indent << UTF8("}\n\n");

		add_parameter(String(UTF8("lighting")), cpt_fragment_color, pqt_out,
			function_locals, function_parameters, uniform_buffers);

		stream << local_indent << cpt_fragment_color << UTF8(" = ");
		stream << sslt_diffuse_colors_sum;

		add_parameter(String(UTF8("lighting")), cpt_albedo, pqt_in,
			function_locals, function_parameters, uniform_buffers);

		stream << UTF8(" * ") << cpt_albedo << UTF8(".rgb");

		stream << UTF8(";\n");

		add_parameter(String(UTF8("lighting")), cpt_specular, pqt_in,
			function_locals, function_parameters, uniform_buffers);

		stream << local_indent << cpt_fragment_color << UTF8(" += ");
		stream << cpt_specular << UTF8(" * ");
		stream << sslt_specular_colors_sum << UTF8(";\n");

		main << indent << UTF8("/* lighting */\n");
		main << indent << UTF8("{\n");

		write_parameters(String(), function_locals, array_sizes,
			local_indent, String(), main);

		main << stream.str() << indent << UTF8("}\n");

		add_parameters(function_parameters, locals, globals,
			uniform_buffers);
	}

	void ShaderSourceBuilder::build_lights(
		const ParameterSizeTypeUint16Map &array_sizes,
		const ShaderSourceParameterVector &locals,
		const String &indent, const ShaderType shader_type,
		const bool shadow, ShaderSourceBuildData &build_data,
		OutStream &main, OutStream &functions,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		ShaderSourceParameterVector function_locals;
		ShaderSourceParameterVector function_parameters;
		CommonParameterType output_color;
		StringStream stream;
		String local_indent, local_loop_indent;
		String lighting;
		Uint16 light_offset, lights_count;
		bool dynamic_lights_count;

		lighting = UTF8("lighting");
		local_indent = indent.get() + UTF8("\t");
		local_loop_indent = local_indent.get() + UTF8("\t");

		if (shader_type != st_fragment)
		{
			output_color = cpt_vertex_color;
			light_offset = get_fragment_lights_count();
			lights_count = build_data.get_lights_count();
			dynamic_lights_count = true;
		}
		else
		{
			output_color = cpt_fragment_color;
			light_offset = 0;
			lights_count = build_data.get_fragment_lights_count();
			dynamic_lights_count =
				get_dynamic_lights_count();
		}

		add_local(String(UTF8("lighting")), sslt_i, pqt_out,
			function_locals, function_parameters,
			uniform_buffers);
		add_local(String(UTF8("lighting")), sslt_shadow_values,
			pqt_out, function_locals, function_parameters,
			uniform_buffers);

		stream << UTF8("\n");

		if (build_data.get_shader_build() == sbt_debug_diffuse_light)
		{
			add_parameter(String(UTF8("lighting")),
				sslt_diffuse_colors_sum, pqt_out,
				function_locals, function_parameters,
				uniform_buffers);
		}
		else
		{
			add_local(String(UTF8("lighting")),
				sslt_diffuse_colors_sum, pqt_out,
				function_locals, function_parameters,
				uniform_buffers);
		}

		stream << local_indent << sslt_diffuse_colors_sum;
		stream << UTF8(" = ");

		if ((shader_type != st_fragment) ||
			(build_data.get_vertex_lights_count() == 0))
		{
			add_parameter(String(UTF8("lighting")),
				apt_sky_ground_hemispheres, function_locals,
				function_parameters, uniform_buffers);

			stream << apt_sky_ground_hemispheres;
			stream << UTF8("[0].rgb + ");
			stream << apt_sky_ground_hemispheres;
			stream << UTF8("[1].rgb * ");
			stream << UTF8("(0.5 * ");

			if (shader_type == st_fragment)
			{
				stream << cpt_fragment_normal;
			}
			else
			{
				stream << cpt_world_normal;
			}

			stream << UTF8(".z + 0.5);\n");
		}
		else
		{
			add_parameter(String(UTF8("lighting")),
				cpt_vertex_color, pqt_in, function_locals,
				function_parameters, uniform_buffers);

			stream << cpt_vertex_color << UTF8(";\n");
		}

		if (shader_type == st_fragment)
		{
			add_parameter(String(UTF8("lighting")), cpt_emission,
				pqt_in, function_locals, function_parameters,
				uniform_buffers);


			if (build_data.get_shader_build() == 
				sbt_debug_specular_light)
			{
				add_parameter(String(UTF8("lighting")),
					sslt_specular_colors_sum, pqt_out,
					function_locals, function_parameters,
					uniform_buffers);
			}
			else
			{
				add_local(String(UTF8("lighting")),
					sslt_specular_colors_sum, pqt_out,
					function_locals, function_parameters,
					uniform_buffers);
			}

			stream << local_indent << sslt_diffuse_colors_sum;
			stream << UTF8(" += ") << cpt_emission << UTF8(";\n");

			stream << local_indent << sslt_specular_colors_sum;
			stream << UTF8(" = vec3(0.0);\n");
		}

		if ((shader_type == st_fragment) && shadow)
		{
			add_parameter(String(UTF8("lighting")), cpt_shadow,
				pqt_in, function_locals, function_parameters,
				uniform_buffers);

			stream << local_indent << sslt_shadow_values;
			stream << UTF8(" = vec3(") << cpt_shadow;
			stream << UTF8(" * ") << get_shadow_scale();
			stream << UTF8(" + ") << (1.0 - get_shadow_scale());
			stream << UTF8(", ") << cpt_shadow;
			stream << UTF8(", 1.0);\n");
		}

		stream << UTF8("\n");
		stream << local_indent << UTF8("for (") << sslt_i;
		stream << UTF8(" = ") << light_offset << UTF8("; ") << sslt_i;
		stream << UTF8(" < ");

		if (dynamic_lights_count && (shader_type == st_fragment))
		{
			add_parameter(String(UTF8("lighting")),
				apt_dynamic_lights_count, function_locals,
				function_parameters, uniform_buffers);

			stream << apt_dynamic_lights_count;
		}
		else
		{
			stream << lights_count;
		}

		stream << UTF8("; ++") << sslt_i << UTF8(")\n");
		stream << local_indent << UTF8("{\n");

		add_local(String(UTF8("lighting")), cpt_light_color, pqt_in,
			function_locals, function_parameters, uniform_buffers);
		add_parameter(String(UTF8("lighting")), apt_light_colors,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << cpt_light_color;
		stream << UTF8(" = ") << apt_light_colors;
		stream << UTF8("[") << sslt_i << UTF8("];\n");

		add_local(String(UTF8("lighting")), cpt_light_position, pqt_in,
			function_locals, function_parameters, uniform_buffers);
		add_parameter(String(UTF8("lighting")), apt_light_positions,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << cpt_light_position;
		stream << UTF8(" = ") << apt_light_positions;
		stream << UTF8("[") << sslt_i << UTF8("];\n");

		build_function(array_sizes, function_locals, local_loop_indent,
			sst_light, shader_type, build_data, stream, functions,
			function_parameters, uniform_buffers);

		make_parameter_local(
			CommonParameterUtil::get_str(cpt_diffuse_color),
			function_locals, function_parameters);
		make_parameter_local(
			CommonParameterUtil::get_str(cpt_specular_color),
			function_locals, function_parameters);

		add_local(String(UTF8("lighting")), cpt_diffuse_color, pqt_in,
			function_locals, function_parameters, uniform_buffers);

		stream << local_loop_indent << sslt_diffuse_colors_sum;
		stream << UTF8(" += ") << cpt_diffuse_color;

		if ((shader_type == st_fragment) && shadow)
		{
			add_parameter(String(UTF8("lighting")), cpt_shadow,
				pqt_in, function_locals, function_parameters,
				uniform_buffers);

			stream << UTF8(" * ") << sslt_shadow_values;
			stream << UTF8(".x");
		}

		stream << UTF8(";\n");

		if (shader_type == st_fragment)
		{
			add_local(String(UTF8("lighting")), cpt_specular_color,
				pqt_in, function_locals, function_parameters,
				uniform_buffers);

			stream << local_loop_indent << sslt_specular_colors_sum;
			stream << UTF8(" += ") << cpt_specular_color;

			if (shadow)
			{
				stream << UTF8(" * ") << sslt_shadow_values;
				stream << UTF8(".y");
			}

			stream << UTF8(";\n");
		}

		if ((shader_type == st_fragment) && shadow)
		{
			stream << local_loop_indent << sslt_shadow_values;
			stream << UTF8(".xy = ") << sslt_shadow_values;
			stream << UTF8(".zz;\n");
		}

		stream << local_indent << UTF8("}\n\n");

		add_parameter(String(UTF8("lighting")), output_color, pqt_out,
			function_locals, function_parameters, uniform_buffers);

		stream << local_indent << output_color << UTF8(" = ");
		stream << sslt_diffuse_colors_sum;

		if (shader_type == st_fragment)
		{
			add_parameter(String(UTF8("lighting")), cpt_albedo,
				pqt_in, function_locals, function_parameters,
				uniform_buffers);

			stream << UTF8(" * ") << cpt_albedo << UTF8(".rgb");
		}

		stream << UTF8(";\n");

		if (shader_type == st_fragment)
		{
			add_parameter(String(UTF8("lighting")), cpt_specular,
				pqt_in, function_locals, function_parameters,
				uniform_buffers);

			stream << local_indent << output_color << UTF8(" += ");
			stream << cpt_specular << UTF8(" * ");
			stream << sslt_specular_colors_sum << UTF8(";\n");
		}

		main << indent << UTF8("/* lighting */\n");
		main << indent << UTF8("{\n");

		write_parameters(String(), function_locals, array_sizes,
			local_indent, String(), main);

		main << stream.str() << indent << UTF8("}\n");

		add_parameters(function_parameters, locals, globals,
			uniform_buffers);
	}

	void ShaderSourceBuilder::build_vertex_source(
		const ParameterSizeTypeUint16Map &array_sizes,
		ShaderSourceBuildData &build_data, OutStream &main,
		OutStream &functions, ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		ShaderSourceParameterVector locals;
		String gl_Position, indent;

		indent = UTF8("\t");
		gl_Position = UTF8("gl_Position");

		if (build_data.get_option(ssbot_tangent))
		{
			build_function(array_sizes, locals, indent,
				sst_world_tangent_transformation, st_vertex,
				build_data, main, functions, globals,
				uniform_buffers);
		}
		else
		{
			if (build_data.get_option(ssbot_normal))
			{
				build_function(array_sizes, locals, indent,
					sst_world_normal_transformation,
					st_vertex, build_data, main, functions,
					globals, uniform_buffers);
			}
			else
			{
				build_function(array_sizes, locals, indent,
					sst_world_depth_transformation,
					st_vertex, build_data, main, functions,
					globals, uniform_buffers);
			}
		}

		if ((build_data.get_shader_build() == sbt_glow) &&
			build_data.get_option(ssbot_normal))
		{
			add_parameter(String(UTF8("vertex")),
				cpt_world_position, pqt_inout, locals, globals,
				uniform_buffers);
			add_parameter(String(UTF8("vertex")), cpt_world_normal,
				pqt_in, locals, globals, uniform_buffers);

			main << indent << UTF8("/* glow */\n");
			main << indent << cpt_world_position << UTF8(" += ");
			main << cpt_world_normal << UTF8(";\n");
		}

		add_parameter(String(UTF8("vertex")),
			apt_projection_view_matrix, locals, globals,
			uniform_buffers);
		add_parameter(String(UTF8("vertex")), cpt_world_position,
			pqt_in, locals, globals, uniform_buffers);

		main << indent << UTF8("/* gl_Position */\n");
		main << indent << gl_Position;
		main << UTF8(" = ") << apt_projection_view_matrix;
		main << UTF8(" * vec4(") << cpt_world_position;
		main << UTF8(", 1.0);\n");

		if (build_data.get_version() < svt_130)
		{		
			add_parameter(String(UTF8("lighting")),
				sslt_project_space, pqt_out, locals, globals,
				uniform_buffers);

			main << indent << UTF8("/* projection space position");
			main << UTF8(" */\n");
			main << indent << sslt_project_space;
			main << UTF8(" = vec3((") << gl_Position;
			main << UTF8(".xy + ") << gl_Position;
			main << UTF8(".w) * 0.5, ") << gl_Position;
			main << UTF8(".w);\n");
		}

		if (build_data.get_option(ssbot_view_position))
		{
			build_function(array_sizes, locals, indent,
				sst_view_transformation, st_vertex, build_data,
				main, functions, globals, uniform_buffers);
		}

		if ((build_data.get_shader_build() == sbt_default) ||
			(build_data.get_shader_build() == sbt_glow) ||
			(build_data.get_shader_build() == sbt_debug_shadow) ||
			(build_data.get_shader_build() ==
				sbt_debug_diffuse_light) ||
			(build_data.get_shader_build() ==
				sbt_debug_specular_light))
		{
			if (build_data.get_option(ssbot_fog))
			{
				build_function(array_sizes, locals, indent,
					sst_fog, st_vertex, build_data, main,
					functions, globals, uniform_buffers);
			}
		}

		if ((build_data.get_shader_build() == sbt_default) ||
			(build_data.get_shader_build() == sbt_debug_shadow) ||
			(build_data.get_shader_build() ==
				sbt_debug_diffuse_light) ||
			(build_data.get_shader_build() ==
				sbt_debug_specular_light))
		{
			if ((build_data.get_vertex_lights_count() > 0) &&
				build_data.get_option(ssbot_lighting))
			{
				build_lights(array_sizes, locals, indent,
					st_vertex, false, build_data, main,
					functions, globals, uniform_buffers);
			}

			if ((build_data.get_shadow_maps_count() > 0) &&
				(build_data.get_fragment_lights_count() > 0))
			{
				build_function(array_sizes, locals, indent,
					sst_shadow_uv, st_vertex, build_data,
					main, functions, globals,
					uniform_buffers);
			}
		}

		build_function(array_sizes, locals, indent, sst_uv, st_vertex,
			build_data, main, functions, globals, uniform_buffers);

		if (build_data.get_option(ssbot_node_based_effect))
		{
			build_function(array_sizes, locals, indent,
				sst_main_effect, st_vertex, build_data, main,
				functions, globals, uniform_buffers);
		}
	}

	void ShaderSourceBuilder::build_geometry_source(
		const ParameterSizeTypeUint16Map &array_sizes,
		const ShaderSourceParameterVector &varyings,
		const String &in_prefix, const String &out_prefix,
		const bool use_block, ShaderSourceBuildData &build_data,
		OutStream &main, OutStream &functions,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
#if	0
		ShaderSourceParameterVector locals;
		String gl_Position, index, indent;
		Uint16 i, j, count, size;

		gl_Position = UTF8("gl_Position");

		add_parameter(String(UTF8("geometry")), sslt_i, pqt_out,
			locals, globals, uniform_buffers);
		add_parameter(String(UTF8("geometry")), cpt_world_position,
			pqt_in, locals, globals, uniform_buffers);
		add_parameter(String(UTF8("geometry")),
			apt_projection_view_matrix, locals, globals,
			uniform_buffers);

		count = 3;

		indent = UTF8("\t\t");

		main << UTF8("\n");
		main << UTF8("\tfor (") << sslt_i << UTF8(" = 0; ") << sslt_i;
		main << UTF8(" < ") << data.get_layer_count() << UTF8("; ++");
		main << sslt_i << UTF8(")\n") << UTF8("\t{\n");

		main << UTF8("\tgl_PrimitiveID = gl_PrimitiveIDIn * ");
		main << data.get_layer_count() << UTF8(" + ") << sslt_i;
		main << UTF8(";\n");

		for (i = 0; i < count; ++i)
		{
			StringStream str;

			str << i;

			index = String(str.str());

			main << indent << UTF8("gl_Layer = ") << sslt_i;
			main << UTF8(";\n");

			main << indent << gl_Position << UTF8(" = ");
			main << apt_projection_view_matrix << UTF8("[");
			main << sslt_i << UTF8("] * vec4(");

			write_parameter_indexed(in_prefix,
				ShaderSourceParameterBuilder::build(
					String(UTF8("geometry")),
					cpt_world_position, pqt_in),
				String(), index, use_block, main);

			main << UTF8(", 1.0);\n");

			main << indent << UTF8("/* copying vertex output to ");
			main << UTF8("fragment input */\n");

			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				varyings)
			{
				if (parameter.get_size() == 1)
				{
					write_parameter(out_prefix, parameter,
						indent, use_block, main);

					write_parameter_indexed(in_prefix,
						parameter, String(UTF8(" = ")),
						index, use_block, main);

					main << UTF8(";\n");

					continue;
				}

				size = parameter.get_size();

				for (j = 0; j < size; ++j)
				{
					StringStream str;

					str << UTF8("[") << j << UTF8("]");

					write_parameter(out_prefix, parameter,
						indent, use_block, main);

					main << str.str();

					write_parameter_indexed(in_prefix,
						parameter, String(UTF8(" = ")),
						index, use_block, main);

					main << str.str() << UTF8(";\n");
				}
			}

			main << UTF8("\n");
			main << UTF8("\t\tEmitVertex();\n");
			main << UTF8("\n");
		}

		main << UTF8("\n");
		main << UTF8("\t\tEndPrimitive();\n");
		main << UTF8("\t}\n");
#endif
	}

	void ShaderSourceBuilder::build_fragment_source(
		const ParameterSizeTypeUint16Map &array_sizes,
		ShaderSourceBuildData &build_data,
		OutStream &main, OutStream &functions,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		ShaderSourceParameterVector locals;
		ShaderSourceParameter tmp;
		CommonParameterType output_parameter;
		StringArray8 output_array;
		String output, indent;
		Uint16 i, count;
		bool shadows;

		indent = UTF8("\t");

		if (build_data.get_version() > svt_150)
		{
			output = UTF8("FragColor");
		}
		else
		{
			output = UTF8("gl_FragColor");
		}

		count = output_array.size();

		for (i = 0; i < count; ++i)
		{
			StringStream str;

			if (build_data.get_version() > svt_150)
			{
				str << UTF8("FragData_") << i;
			}
			else
			{
				str << UTF8("gl_FragData[") << i << UTF8("]");
			}

			output_array[i] = String(str.str());
		}

		shadows = false;

		if (build_data.get_option(ssbot_shadow_uv_ddx_ddy))
		{
			build_function(array_sizes, locals, indent,
				sst_shadow_uv_ddx_ddy, st_fragment, build_data,
				main, functions, globals, uniform_buffers);
		}

		if (build_data.get_option(ssbot_view_direction))
		{
			build_function(array_sizes, locals, indent,
				sst_view_direction, st_fragment, build_data,
				main, functions, globals, uniform_buffers);
		}

		if (build_data.get_option(ssbot_tbn_matrix))
		{
			build_function(array_sizes, locals, indent,
				sst_tbn_matrix, st_fragment, build_data, main,
				functions, globals, uniform_buffers);
		}

		build_function(array_sizes, locals, indent, sst_main_effect,
			st_fragment, build_data, main, functions, globals,
			uniform_buffers);

		if (build_data.get_option(ssbot_transparent) &&
			build_data.get_option(ssbot_alpha_test))
		{
			add_parameter(String(UTF8("fragment")),	cpt_albedo,
				pqt_in, locals, globals, uniform_buffers);

			main << indent << UTF8("if (") << cpt_albedo;
			main << UTF8(".a < 0.5)\n") << indent;
			main << UTF8("{\n") << indent;
			main << UTF8("\tdiscard;\n") << indent;
			main << UTF8("}\n");
		}

		if ((build_data.get_shader_build() == sbt_default) ||
			(build_data.get_shader_build() == sbt_debug_shadow) ||
			(build_data.get_shader_build() ==
				sbt_debug_diffuse_light) ||
			(build_data.get_shader_build() ==
				sbt_debug_specular_light) ||
			(build_data.get_shader_build() ==
				sbt_debug_packed_light_index) ||
			(build_data.get_shader_build() ==
				sbt_debug_light_index))
		{
			if ((build_data.get_fragment_lights_count() > 0) &&
				build_data.get_option(ssbot_lighting))
			{
				if (build_data.get_shadow_maps_count() > 0)
				{
					shadows = build_function(array_sizes,
						locals, indent,
						sst_shadow_mapping,
						st_fragment, build_data,
						main, functions, globals,
						uniform_buffers);
				}

				if (build_data.get_option(
					ssbot_light_indexed_deferred))
				{
					build_light_index_lights(array_sizes,
						locals, indent, shadows,
						build_data, main, functions,
						globals, uniform_buffers);
				}
				else
				{
					build_lights(array_sizes, locals,
						indent, st_fragment, shadows,
						build_data, main, functions,
						globals, uniform_buffers);
				}
			}
			else
			{
				add_parameter(String(UTF8("fragment")),
					cpt_fragment_color, pqt_out, locals,
					globals, uniform_buffers);
				add_parameter(String(UTF8("fragment")),
					cpt_albedo, pqt_in, locals, globals,
					uniform_buffers);

				main << indent << cpt_fragment_color;
				main << UTF8(" = ") << cpt_albedo;
				main << UTF8(".rgb");

				add_parameter(String(UTF8("fragment")),
					cpt_emission, pqt_in, locals, globals,
					uniform_buffers);

				if (build_data.get_option(ssbot_lighting))
				{
					main << UTF8(" * (");

					if (build_data.get_vertex_lights_count(
						) > 0)
					{
						add_parameter(String(
							UTF8("fragment")),
							cpt_vertex_color,
							pqt_in,	locals,
							globals,
							uniform_buffers);
						main << cpt_vertex_color;
					}
					else
					{
						add_parameter(String(
							UTF8("fragment")),
							apt_sky_ground_hemispheres,
							locals, globals,
							uniform_buffers);

						main << apt_sky_ground_hemispheres;
						main << UTF8("[0].rgb + ");
						main << apt_sky_ground_hemispheres;
						main << UTF8("[1].rgb * 0.5");
					}

					main << UTF8(" + ") << cpt_emission;
					main << UTF8(")");
				}

				main << UTF8(";\n");
			}
		}

		if (build_data.get_shader_build() == sbt_glow)
		{
			add_parameter(String(UTF8("fragment")),
				cpt_fragment_color, pqt_out, locals,
				globals, uniform_buffers);
			add_parameter(String(UTF8("fragment")), cpt_albedo,
				pqt_in, locals, globals, uniform_buffers);

			main << indent << cpt_fragment_color;
			main << UTF8(" = ") << cpt_albedo;
			main << UTF8(".rgb");

			add_parameter(String(UTF8("fragment")),	cpt_emission,
				pqt_in, locals, globals, uniform_buffers);

			main << UTF8(" * (") << cpt_emission << UTF8(" * 0.5");
			main << UTF8(" + 0.5);\n");
		}

		switch (build_data.get_shader_output())
		{
			case sot_int:
				output_parameter = cpt_output_data_int;
				break;
			case sot_uint:
				output_parameter = cpt_output_data_uint;
				break;
			case sot_float:
			default:
				output_parameter = cpt_output_data_float;
				break;
		}

		switch (build_data.get_shader_build())
		{
			case sbt_default:
			case sbt_glow:
				add_parameter(String(UTF8("fragment")),
					cpt_fragment_color, pqt_in, locals,
					globals, uniform_buffers);

				main << indent << output << UTF8(".rgb = ");
				main << UTF8("1.0 - exp(-");

				if (build_data.get_option(ssbot_fog))
				{
					add_parameter(String(UTF8("fragment")),
						cpt_fog, pqt_in, locals,
						globals, uniform_buffers);
					add_parameter(String(UTF8("fragment")),
						apt_fog_data, locals, globals,
						uniform_buffers);

					main << UTF8("mix(") << apt_fog_data;
					main << UTF8(".rgb, ");
					main << cpt_fragment_color;
					main << UTF8(", ");
					main << cpt_fog << UTF8(")");
				}
				else
				{
					main << cpt_fragment_color;
				}

				main << UTF8(");\n");

				break;
			case sbt_depth:
				main << indent << output;
				main << UTF8(".rgb = vec3(1.0);\n");

				break;
			case sbt_height_map:
				tmp = ShaderSourceParameterBuilder::build(
					String(UTF8("fragment")),
					spt_effect_15, pt_sampler2D);

				ShaderSourceParameterBuilder::add_parameter(
					tmp, locals, globals, uniform_buffers);

				add_parameter(String(UTF8("fragment")),
					cpt_world_position, pqt_in, locals,
					globals, uniform_buffers);

				main << indent;

				if (build_data.get_version() > svt_120)
				{
					main << UTF8("if (texelFetch(");
					main << spt_effect_15;
					main << UTF8(", ivec2(");
					main << UTF8("gl_FragCoord.xy), 0)");
					main << UTF8(".r > ");
					main << cpt_world_position;
					main << UTF8(".z)\n");
				}
				else
				{
					add_parameter(String(UTF8("fragment")),
						apt_screen_size, locals,
						globals, uniform_buffers);

					main << UTF8("if (texture2D(");
					main << spt_effect_15;
					main << UTF8(", ");
					main << UTF8("gl_FragCoord.xy *");
					main << apt_screen_size;
					main << UTF8(".zw).r > ");
					main << cpt_world_position;
					main << UTF8(".z)\n");
				}

				main << indent << UTF8("{\n") << indent;
				main << UTF8("\tdiscard;\n") << indent;
				main << UTF8("}\n");

				main << indent << output;
				main << UTF8(" = ") << cpt_world_position;

				if (build_data.get_version() > svt_150)
				{
					main << UTF8(".z;\n");
				}
				else
				{
					main << UTF8(".zzzz;\n");
				}

				break;
			case sbt_shadow:
				build_function(array_sizes, locals, indent,
					sst_shadow_map, st_fragment,
					build_data, main, functions, globals,
					uniform_buffers);

				add_parameter(String(UTF8("fragment")),
					cpt_shadow_map_data, pqt_in, locals,
					globals, uniform_buffers);

				main << indent << output;

				if (build_data.get_option(ssbot_alpha_write))
				{
					main << UTF8(".rgb");
				}

				main << UTF8(" = ") << cpt_shadow_map_data;

				if (!build_data.get_option(ssbot_alpha_write))
				{
					main << UTF8(".r");
				}
				else
				{
					main << UTF8(".rrr");
				}

				main << UTF8(";\n");

				break;
			case sbt_debug_uv:
				add_parameter(String(UTF8("fragment")),
					cpt_world_uv, pqt_in, locals,
					globals, uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << UTF8("vec3(") << cpt_world_uv;
				main << UTF8(", 0.0);\n");
				break;
			case sbt_debug_depth:
				add_parameter(String(UTF8("fragment")),
					apt_z_params, locals, globals,
					uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << UTF8("vec3(2.0 * z_params.x / ");
				main << UTF8("(z_params.z - (2.0 * ");
				main << UTF8("gl_FragCoord.z - 1.0) * ");
				main << UTF8("z_params.w));\n");
				break;
			case sbt_debug_alpha:
				add_parameter(String(UTF8("fragment")),
					cpt_albedo, pqt_in, locals,
					globals, uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << cpt_albedo << UTF8(".aaa;\n");
				break;
			case sbt_screen_quad:
				add_parameter(String(UTF8("fragment")),
					output_parameter, pqt_in, locals,
					globals, uniform_buffers);

				count = build_data.get_render_targets_count();

				for (i = 0; i < count; ++i)
				{
					main << indent << output_array[i];
					main << UTF8(" = ") << output_parameter;
					main << UTF8("[") << i << UTF8("]");
					main << get_output_swizzle(build_data.
						get_output_channel_count(i));
					main << UTF8(";\n");
				}
				break;
			case sbt_debug_albedo:
				add_parameter(String(UTF8("fragment")),
					cpt_albedo, pqt_in, locals,
					globals, uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << cpt_albedo << UTF8(".rgb;\n");
				break;
			case sbt_debug_normal:
				add_parameter(String(UTF8("fragment")),
					cpt_fragment_normal, pqt_in, locals,
					globals, uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << cpt_fragment_normal;
				main << UTF8(" * 0.5 + 0.5;\n");
				break;
			case sbt_debug_tbn_matrix_0:
				add_parameter(String(UTF8("fragment")),
					cpt_tbn_matrix, pqt_in, locals,
					globals, uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << UTF8("tbn_matrix[0] * 0.5 + 0.5;\n");
				break;
			case sbt_debug_tbn_matrix_1:
				add_parameter(String(UTF8("fragment")),
					cpt_tbn_matrix, pqt_in, locals,
					globals, uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << UTF8("tbn_matrix[1] * 0.5 + 0.5;\n");
				break;
			case sbt_debug_tbn_matrix_2:
				add_parameter(String(UTF8("fragment")),
					cpt_tbn_matrix, pqt_in, locals,
					globals, uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << UTF8("tbn_matrix[2] * 0.5 + 0.5;\n");
				break;
			case sbt_debug_shadow:
				main << indent << output << UTF8(".rgb = ");

				if ((build_data.get_fragment_lights_count() ==
					0) ||
					(build_data.get_shadow_maps_count() ==
						0) ||
					!build_data.get_option(ssbot_lighting))
				{
					main << UTF8("vec3(1.0);\n");
					break;
				}

				add_parameter(String(UTF8("fragment")),
					cpt_shadow, pqt_in, locals, globals,
					uniform_buffers);
				main << UTF8("vec3(") << cpt_shadow;
				main << UTF8(");\n");
				break;
			case sbt_debug_specular:
				add_parameter(String(UTF8("fragment")),
					cpt_specular, pqt_in, locals,
					globals, uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << cpt_specular << UTF8(";\n");
				break;
			case sbt_debug_gloss:
				add_parameter(String(UTF8("fragment")),
					cpt_specular, pqt_in, locals,
					globals, uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << UTF8("vec3(") << cpt_gloss;
				main << UTF8(");\n");
				break;
			case sbt_debug_emissive:
				add_parameter(String(UTF8("fragment")),
					cpt_emission, pqt_in, locals,
					globals, uniform_buffers);
				main << indent << output << UTF8(".rgb = ");
				main << cpt_emission << UTF8(";\n");
				break;
			case sbt_debug_diffuse_light:
				main << indent << output << UTF8(".rgb = ");

				if ((build_data.get_fragment_lights_count()
					== 0) ||
					!build_data.get_option(ssbot_lighting))
				{
					main << UTF8("vec3(0.0);\n");
					break;
				}

				add_parameter(String(UTF8("fragment")),
					sslt_diffuse_colors_sum, pqt_in,
					locals, globals, uniform_buffers);
				main << sslt_diffuse_colors_sum << UTF8(";\n");
				break;
			case sbt_debug_specular_light:
				main << indent << output << UTF8(".rgb = ");

				if ((build_data.get_fragment_lights_count()
					== 0) ||
					!build_data.get_option(ssbot_lighting))
				{
					main << UTF8("vec3(0.0);\n");
					break;
				}

				add_parameter(String(UTF8("fragment")),
					sslt_specular_colors_sum, pqt_in,
					locals, globals, uniform_buffers);
				main << sslt_specular_colors_sum << UTF8(";\n");
				break;
			case sbt_debug_packed_light_index:
				main << indent << output << UTF8(".rgb = ");

				if ((build_data.get_fragment_lights_count()
					== 0) ||
					!build_data.get_option(ssbot_lighting))
				{
					main << UTF8("vec3(0.0);\n");
					break;
				}

				add_parameter(String(UTF8("fragment")),
					sslt_light_indices, pqt_in,
					locals, globals, uniform_buffers);
				main << sslt_light_indices;
				main << UTF8(".rgb;\n");
				break;
			case sbt_debug_light_index:
				main << indent << output << UTF8(".rgb = ");

				if ((build_data.get_fragment_lights_count()
					== 0) ||
					!build_data.get_option(ssbot_lighting))
				{
					main << UTF8("vec3(0.0);\n");
					break;
				}

				add_parameter(String(UTF8("fragment")),
					sslt_light_indices, pqt_in,
					locals, globals, uniform_buffers);
				main << sslt_light_indices;
				main << UTF8(".rgb;\n");
				break;
		}

		if (build_data.get_option(ssbot_alpha_write) &&
			(build_data.get_shader_build() != sbt_screen_quad))
		{
			add_parameter(String(UTF8("fragment")), cpt_albedo,
				pqt_in, locals, globals, uniform_buffers);

			main << indent << output << UTF8(".a = ");
			main << cpt_albedo << UTF8(".a;\n");
		}
	}

	bool ShaderSourceBuilder::check_function(const String &name,
		const ShaderSourceType shader_source_type,
		const ShaderType shader_type,
		ShaderSourceBuildData &build_data) const
	{
		AbstractShaderSourceSharedPtr shader_source;

		if (!get_shader_source(shader_source_type, build_data,
			shader_source))
		{
			return false;
		}

		return shader_source->check_source_parameter(shader_type,
			build_data.get_version(), name);
	}

	bool ShaderSourceBuilder::get_source_parameter(
		const CommonParameterType common_parameter,
		ShaderSourceBuildData &build_data) const
	{
		String name;
		bool result;

		name = CommonParameterUtil::get_str(common_parameter);
		result = false;

		if (build_data.get_option(ssbot_view_position))
		{
			result |= check_function(name, sst_view_transformation,
				st_vertex, build_data);
		}

		if ((build_data.get_shader_build() == sbt_default) ||
			(build_data.get_shader_build() == sbt_glow) ||
			(build_data.get_shader_build() == sbt_debug_shadow) ||
			(build_data.get_shader_build() ==
				sbt_debug_diffuse_light) ||
			(build_data.get_shader_build() ==
				sbt_debug_specular_light) ||
			(build_data.get_shader_build() ==
				sbt_debug_packed_light_index) ||
			(build_data.get_shader_build() ==
				sbt_debug_light_index))
		{
			if (build_data.get_option(ssbot_fog))
			{
				result |= check_function(name, sst_fog,
					st_vertex, build_data);
			}

			if (build_data.get_vertex_lights_count() > 0)
			{
				result |= check_function(name, sst_light,
					st_vertex, build_data);
			}

			if ((build_data.get_shadow_maps_count() > 0) &&
				(build_data.get_fragment_lights_count() > 0))
			{
				result |= check_function(name, sst_shadow_uv,
					st_vertex, build_data);
			}
		}

		result |= check_function(name, sst_uv, st_vertex, build_data);

		if (build_data.get_option(ssbot_node_based_effect))
		{
			result |= check_function(name, sst_main_effect,
				st_vertex, build_data);
		}

		if ((build_data.get_shader_build() == sbt_debug_uv) &&
			(common_parameter == cpt_world_uv))
		{
			result = true;
		}

		if (build_data.get_option(ssbot_shadow_uv_ddx_ddy))
		{
			result |= check_function(name, sst_shadow_uv_ddx_ddy,
				st_fragment, build_data);
		}

		if (build_data.get_option(ssbot_view_direction))
		{
			result |= check_function(name, sst_view_direction,
				st_fragment, build_data);
		}

		if (build_data.get_option(ssbot_tbn_matrix) ||
			(build_data.get_shader_build() ==
				sbt_debug_tbn_matrix_0) ||
			(build_data.get_shader_build() ==
				sbt_debug_tbn_matrix_1) ||
			(build_data.get_shader_build() ==
				sbt_debug_tbn_matrix_2))
		{
			result |= check_function(name, sst_tbn_matrix,
				st_fragment, build_data);
		}

		result |= check_function(name, sst_main_effect, st_fragment,
			build_data);

		if (common_parameter == cpt_albedo)
		{
			result = true;
		}

		if (((build_data.get_shader_build() == sbt_default) &&
			(build_data.get_fragment_lights_count() > 0)) ||
			(build_data.get_shader_build() == sbt_debug_shadow) ||
			(build_data.get_shader_build() ==
				sbt_debug_diffuse_light) ||
			(build_data.get_shader_build() ==
				sbt_debug_specular_light))
		{
			if (build_data.get_shadow_maps_count() > 0)
			{
				result |= check_function(name,
					sst_shadow_mapping, st_fragment,
					build_data);
			}

			result |= check_function(name, sst_light, st_fragment,
				build_data);
		}

		if (build_data.get_shader_build() == sbt_shadow)
		{
			result |= check_function(name, sst_shadow_map,
				st_fragment, build_data);
		}

		if (result)
		{
			LOG_DEBUG(lt_shader_source, UTF8("Common parameter "
				"'%1%' used."), name);
		}
		else
		{
			LOG_DEBUG(lt_shader_source, UTF8("Common parameter "
				"'%1%' not used."), name);
		}

		return result;
	}

	void ShaderSourceBuilder::set_shadow_map_type(const String &name)
	{
		m_default_sources[sst_shadow_mapping] = name;
		m_default_sources[sst_shadow_map] = name;
		m_default_sources[sst_shadow_uv] = name;
	}

	ShaderSourceTypeStringMap ShaderSourceBuilder::build_sources(
		const EffectDescription &description) const
	{
		ShaderSourceTypeStringMap::iterator found;
		ShaderSourceTypeStringMap sources;

		sources = get_default_sources();

		sources[sst_world_depth_transformation] =
			description.get_world_transformation();
		sources[sst_world_normal_transformation] =
			description.get_world_transformation();
		sources[sst_world_tangent_transformation] =
			description.get_world_transformation();
		sources[sst_uv] = description.get_texture_coodrinates();
		sources[sst_main_effect] = description.get_main();
		sources[sst_light] = description.get_lighting();

		if (!description.get_receives_shadows())
		{
			found = sources.find(sst_shadow_uv);

			if (found != sources.end())
			{
				sources.erase(found);
			}

			found = sources.find(sst_shadow_mapping);

			if (found != sources.end())
			{
				sources.erase(found);
			}
		}

		if (description.get_lighting().get().empty())
		{
			found = sources.find(sst_light);

			if (found != sources.end())
			{
				sources.erase(found);
			}
		}

		return sources;
	}

	void ShaderSourceBuilder::build(const EffectDescription &description,
		const ShaderBuildType shader_build,
		const ShaderOutputType shader_output,
		const Uint16 lights_count,
		ShaderTypeStringMap &program_description) const
	{
		ShaderSourceBuildData build_data;
		ShaderSourceParameterVector attributes, varyings;
		ShaderSourceParameterVector geometry_in_parameters;
		ShaderSourceParameterVector geometry_out_parameters;
		ShaderSourceParameterVector vertex_globals;
		ShaderSourceParameterVector geometry_globals;
		ShaderSourceParameterVector fragment_globals;
		ParameterSizeTypeUint16Map array_sizes;
		ShaderSourceTypeStringMap sources;
		StringStream vertex_main, geometry_main, fragment_main;
		StringStream vertex_functions, geometry_functions;
		StringStream fragment_functions;
		StringStream vertex_source, geometry_source, fragment_source;
		StringStream version_stream;
		UniformBufferUsage vertex_uniform_buffers;
		UniformBufferUsage fragment_uniform_buffers;
		Uint8Array4 output_channels;
		String vertex_data, fragment_data, prefix, name_prefix;
		String vertex, geometry, fragment;
		ShaderVersionType version_type;
		Uint16 i, render_target_count;
		bool use_block, use_in_out;

		sources = build_sources(description);

		program_description.clear();

		version_type = get_shader_version_type(get_global_vars(
			)->get_opengl_version());

		LOG_DEBUG(lt_shader_source, UTF8("GLSL version %1%"),
			version_type);

		if (version_type >= svt_140)
		{
			vertex_uniform_buffers = UniformBufferUsage(
				get_uniform_buffer_description_cache());

			fragment_uniform_buffers = UniformBufferUsage(
				get_uniform_buffer_description_cache());
		}

		version_stream << UTF8("#version ") << version_type;
		version_stream << UTF8("\n");

		render_target_count =
			EffectDescriptionUtil::get_render_targets_count(
				description.get_description());

		output_channels = description.get_output_channels();

		if (build_data.get_version() <= svt_150)
		{
			BOOST_FOREACH(Uint8 &output_channel, output_channels)
			{
				if (output_channel > 0)
				{
					output_channel = 4;
				}
			}
		}

		build_data = ShaderSourceBuildData(sources, version_type,
			shader_build, shader_output, output_channels,
			get_vertex_lights_count(), get_fragment_lights_count(),
			std::min(get_lights_count(), lights_count),
			get_global_vars()->get_shadow_map_count(),
			render_target_count);

		use_block = (build_data.get_version() >= svt_150) &&
			get_global_vars()->get_use_block();
		use_in_out = (build_data.get_version() >= svt_130) &&
			get_global_vars()->get_use_in_out();

		array_sizes[pst_lights_count] = std::min(
			build_data.get_lights_count(),
			ParameterSizeUtil::get_max_size(pst_lights_count));
		array_sizes[pst_bones_count] = std::min(get_bones_count(),
			ParameterSizeUtil::get_max_size(pst_bones_count));
		array_sizes[pst_shadow_maps_count] = std::min(
			build_data.get_shadow_maps_count(),
			ParameterSizeUtil::get_max_size(pst_shadow_maps_count));
		array_sizes[pst_clipmap_terrain_slices] = std::min(
			get_global_vars()->get_clipmap_terrain_slices(),
			ParameterSizeUtil::get_max_size(
				pst_clipmap_terrain_slices));
		array_sizes[pst_render_targets] =
			EffectDescriptionUtil::get_render_targets_count(
				description.get_description());

		build_data.set_option(ssbot_transparent,
			description.get_transparent());
		build_data.set_option(ssbot_view_direction,
			get_source_parameter(cpt_world_view_direction,
			build_data));
		build_data.set_option(ssbot_shadow_uv_ddx_ddy,
			get_source_parameter(cpt_shadow_uv_ddx_ddy,
			build_data));
		build_data.set_option(ssbot_tbn_matrix, get_source_parameter(
			cpt_tbn_matrix, build_data));
		build_data.set_option(ssbot_view_position,
			get_source_parameter(cpt_view_position, build_data));
		build_data.set_option(ssbot_normal, get_source_parameter(
			cpt_world_normal, build_data));
		build_data.set_option(ssbot_tangent, get_source_parameter(
			cpt_world_tangent, build_data));
		build_data.set_option(ssbot_transparent,
			description.get_transparent());
		build_data.set_option(ssbot_alpha_test,
			!(get_global_vars()->get_exponential_shadow_maps() &&
			get_global_vars()->get_use_multisample_shadows() &&
			(shader_build == sbt_shadow)));
		build_data.set_option(ssbot_alpha_write,
			(build_data.get_version() < svt_330) ||
			(description.get_transparent() &&
			get_global_vars()->get_exponential_shadow_maps() &&
			get_global_vars()->get_use_multisample_shadows() &&
			(shader_build == sbt_shadow)) ||
			(shader_build == sbt_glow) ||
			((shader_build == sbt_default) &&
			description.get_transparency()));
		build_data.set_option(ssbot_use_functions,
			get_global_vars()->get_use_functions());
		build_data.set_option(ssbot_lighting,
			!description.get_lighting().get().empty());
		build_data.set_option(ssbot_light_indexed_deferred,
			get_global_vars()->get_light_system() != lst_default);

		build_fragment_source(array_sizes, build_data, fragment_main,
			fragment_functions, fragment_globals,
			fragment_uniform_buffers);
		build_vertex_source(array_sizes, build_data, vertex_main,
			vertex_functions, vertex_globals,
			vertex_uniform_buffers);

		build_in_out(fragment_globals, vertex_globals, varyings);
		build_attributes(vertex_globals, attributes);

		vertex_data = String(UTF8("data"));
		fragment_data = vertex_data;

		if (!use_block)
		{
			vertex_data = String(vertex_data.get() + UTF8("_"));
			fragment_data = String(fragment_data.get() + UTF8("_"));
		}

		vertex_source << version_stream.str();
		vertex_source << UTF8("/* ") << description.get_name();
		vertex_source << UTF8(" ") <<  shader_build << UTF8( "*/\n");
		vertex_source << UTF8("invariant gl_Position; /* make ");
		vertex_source << UTF8("existing gl_Position be invariant */\n");

		vertex = String(vertex_source.str());

		build_constants(array_sizes, vertex_source);
		vertex_source << UTF8("\n");

		vertex_source << UTF8("/* functions */\n");
		build_decode_normal(String(), vertex_source);
		build_decode_terrain_displacement(String(), vertex_source);
		vertex_source << vertex_functions.str();
		vertex_source << UTF8("\n");

		vertex_source << UTF8("/* vertex shader input */\n");

		if (build_data.get_version() > svt_150)
		{
			write_attributes(String(), attributes, array_sizes,
				vertex_source);
		}
		else
		{
			if (use_in_out)
			{
				prefix = UTF8("in ");
			}
			else
			{
				prefix = UTF8("attribute ");
			}

			write_parameters(String(), attributes, array_sizes,
				prefix, String(), vertex_source);
		}

		vertex_source << UTF8("\n");
		vertex_source << UTF8("/* uniforms */\n");
		write_parameters(String(), pqt_in, vertex_globals, array_sizes,
			String(UTF8("uniform ")), String(), vertex_source);

		vertex_source << UTF8("\n");
		vertex_source << UTF8("/* uniform buffers */\n");
		vertex_uniform_buffers.write(String(), vertex_source);

		vertex_source << UTF8("\n");
		vertex_source << UTF8("/* vertex shader output */\n");

		if (use_block)
		{
			if ((varyings.size() > 0) ||
				(geometry_in_parameters.size() > 0))
			{
				vertex_source << UTF8("out ") << vertex_data;
				vertex_source << UTF8("_block\n{\n");
				write_parameters(String(), varyings,
					array_sizes, String(UTF8("\t")),
					String(), vertex_source);
				write_parameters(String(),
					geometry_in_parameters,
					array_sizes, String(UTF8("\t")),
					String(), vertex_source);
				vertex_source << UTF8("}");
				vertex_source << UTF8(";\n");
			}
		}
		else
		{
			if (use_in_out)
			{
				prefix = UTF8("out ");
			}
			else
			{
				prefix = UTF8("varying ");
			}

			write_parameters(String(), varyings, array_sizes,
				prefix, String(), vertex_source);
			write_parameters(String(), geometry_in_parameters, 
				array_sizes, prefix, String(), vertex_source);
		}

		vertex_source << UTF8("\n");
		vertex_source << UTF8("void main()\n");
		vertex_source << UTF8("{\n");
		write_parameters(String(), pqt_out, vertex_globals,
			array_sizes, String(UTF8("\t")), String(),
			vertex_source);

		vertex_source << UTF8("\n");
		vertex_source << vertex_main.str();
		vertex_source << UTF8("\n");

		vertex_source << UTF8("}\n");

		LOG_DEBUG(lt_shader_source, UTF8("Vertex Shader:\n%1%"),
			vertex_source.str());

		geometry_source << version_stream.str();
		geometry_source << UTF8("/* ") << description.get_name();
		geometry_source << UTF8(" ") <<  shader_build << UTF8( "*/\n");
		geometry_source << UTF8("layout(points) in;\n");
		geometry_source << UTF8("layout(triangle_strip) out;\n");
		geometry_source << UTF8("layout(max_vertices = 4) out;\n");

		build_constants(array_sizes, geometry_source);

		geometry_source << UTF8("\n");

		geometry_source << UTF8("/* functions */\n");
		geometry_source << geometry_functions.str();
		geometry_source << UTF8("\n");

		geometry_source << UTF8("/* geometry shader input */\n");

		if (use_block)
		{
			if ((varyings.size() > 0) ||
				(geometry_in_parameters.size() > 0))
			{
				geometry_source << UTF8("in ") << vertex_data;
				geometry_source << UTF8("_block\n{\n");
				write_parameters(String(), varyings,
					array_sizes, String(UTF8("\t")),
					String(), geometry_source);
				write_parameters(String(),
					geometry_in_parameters, array_sizes,
					String(UTF8("\t")), String(),
					geometry_source);
				geometry_source << UTF8("} ") << vertex_data;
				geometry_source << UTF8("[];\n");
			}
		}
		else
		{
			if (use_in_out)
			{
				prefix = UTF8("in ");
			}
			else
			{
				prefix = UTF8("varying in ");
			}

			write_parameters(vertex_data, varyings, array_sizes,
				prefix, String(UTF8("[]")), geometry_source);
			write_parameters(vertex_data, geometry_in_parameters,
				array_sizes, prefix, String(UTF8("[]")),
				geometry_source);
		}

		geometry_source << UTF8("\n");
		geometry_source << UTF8("/* geometry shader output */\n");

		if (use_block)
		{
			if ((varyings.size() > 0) ||
				(geometry_out_parameters.size() > 0))
			{
				geometry_source << UTF8("out ");
				geometry_source << fragment_data;
				geometry_source << UTF8("_block\n{\n");
				write_parameters(String(), varyings,
					array_sizes, String(UTF8("\t")),
					String(), geometry_source);
				write_parameters(String(),
					geometry_out_parameters, array_sizes,
					String(UTF8("\t")), String(),
					geometry_source);
				geometry_source << UTF8("} ") << fragment_data;
				geometry_source << UTF8(";\n");
			}
		}
		else
		{
			if (use_in_out)
			{
				prefix = UTF8("out ");
			}
			else
			{
				prefix = UTF8("varying out ");
			}

			write_parameters(fragment_data, varyings, array_sizes,
				prefix, String(), geometry_source);
			write_parameters(fragment_data, geometry_out_parameters,
				array_sizes, prefix, String(), geometry_source);
		}

		geometry_source << UTF8("\n");
		write_parameters(String(), pqt_in, geometry_globals,
			array_sizes, String(UTF8("uniform ")), String(),
			geometry_source);
		geometry_source << UTF8("\n");
		geometry_source << UTF8("\n");
		geometry_source << UTF8("void main()\n");
		geometry_source << UTF8("{\n");
		write_parameters(String(), pqt_out, geometry_globals,
			array_sizes, String(UTF8("\t")), String(),
			geometry_source);
		geometry_source << UTF8("\n");
		geometry_source << geometry_main.str();
		geometry_source << UTF8("}\n");

		fragment_source << version_stream.str();
		fragment_source << UTF8("/* ") << description.get_name();
		fragment_source << UTF8(" ") <<  shader_build << UTF8( "*/\n");

		fragment = String(fragment_source.str());

		build_constants(array_sizes, fragment_source);
		fragment_source << UTF8("\n");

		fragment_source << UTF8("/* functions */\n");
		build_decode_normal(String(), fragment_source);
		build_decode_terrain_displacement(String(), fragment_source);
		fragment_source << fragment_functions.str();
		fragment_source << UTF8("\n");

		fragment_source << UTF8("/* uniforms */\n");
		write_parameters(String(), pqt_in, fragment_globals,
			array_sizes, String(UTF8("uniform ")), String(),
			fragment_source);
		fragment_source << UTF8("\n");
		fragment_source << UTF8("/* uniform buffers */\n");
		fragment_uniform_buffers.write(String(), fragment_source);

		fragment_source << UTF8("\n");
		fragment_source << UTF8("/* fragment shader input */\n");

		if (use_block)
		{
			if ((varyings.size() > 0) ||
				(geometry_out_parameters.size() > 0))
			{
				fragment_source << UTF8("in ") << fragment_data;
				fragment_source << UTF8("_block\n{\n");
				write_parameters(String(), varyings,
					array_sizes, String(UTF8("\t")),
					String(), fragment_source);
				write_parameters(String(),
					geometry_out_parameters, array_sizes,
					String(UTF8("\t")), String(),
					fragment_source);
				fragment_source << UTF8("}");

				fragment_source << UTF8(";\n");
			}
		}
		else
		{
			if (use_in_out)
			{
				prefix =  UTF8("in ");
			}
			else
			{
				prefix =  UTF8("varying ");
			}

			write_parameters(String(), varyings, array_sizes,
				prefix, String(), fragment_source);
			write_parameters(String(), geometry_out_parameters,
				array_sizes, prefix, String(), fragment_source);
		}

		if (build_data.get_version() > svt_150)
		{
			fragment_source << UTF8("\n");

			if (description.get_description() != edt_default)
			{
				for (i = 0; i < render_target_count; ++i)
				{
					fragment_source << UTF8("layout(");
					fragment_source << UTF8("location = ");
					fragment_source << i;
					fragment_source << UTF8(") out ");
					fragment_source << get_output_type(
						build_data.get_shader_output(),
						build_data.get_output_channel_count(
							i));
					fragment_source << UTF8(" FragData_");
					fragment_source << i << UTF8(";\n");
				}
			}
			else
			{
				fragment_source << UTF8("layout(location = 0");
				fragment_source << UTF8(") out ");

				if (shader_build == sbt_height_map)
				{
					fragment_source << UTF8("float");
				}
				else
				{
					if (build_data.get_option(
						ssbot_alpha_write))
					{
						fragment_source << UTF8("vec4");
					}
					else
					{
						if (shader_build == sbt_shadow)
						{
							fragment_source <<
								UTF8("float");
						}
						else
						{
							fragment_source <<
								UTF8("vec3");
						}
					}
				}

				fragment_source << UTF8(" FragColor;\n");
			}
		}

		fragment_source << UTF8("\n");
		fragment_source << UTF8("void main()\n");
		fragment_source << UTF8("{\n");
		write_parameters(String(), pqt_out, fragment_globals,
			array_sizes, String(UTF8("\t")), String(),
			fragment_source);

		fragment_source << UTF8("\n");
		fragment_source << fragment_main.str();
		fragment_source << UTF8("}\n");

		LOG_DEBUG(lt_shader_source, UTF8("Fragment Shader:\n%1%"),
			fragment_source.str());

		geometry = UTF8("");

		if (get_global_vars()->get_optmize_shader_source() &&
			(version_type == svt_120))
		{
			try
			{
				vertex = get_optimized_source(
					m_optimizer->get_optimizer(), vertex,
					kGlslOptShaderVertex,
					String(vertex_source.str()));
			}
			catch (boost::exception &exception)
			{
				exception << errinfo_vertex_shader_source(
					vertex_source.str());

				throw;
			}

			LOG_DEBUG(lt_shader_source, UTF8("Vertex Shader:\n%1%"),
				vertex);

			try
			{
				fragment = get_optimized_source(
					m_optimizer->get_optimizer(), fragment,
					kGlslOptShaderFragment,
					String(fragment_source.str()));
			}
			catch (boost::exception &exception)
			{
				exception << errinfo_fragment_shader_source(
					fragment_source.str());

				throw;
			}

			LOG_DEBUG(lt_shader_source,
				UTF8("Fragment Shader:\n%1%"), fragment);
		}
		else
		{
			vertex = vertex_source.str();
			fragment = fragment_source.str();
		}

		program_description[st_vertex] = vertex;
		program_description[st_geometry] = geometry;
		program_description[st_fragment] = fragment;
	}

	StringVector ShaderSourceBuilder::get_shader_source_names(
		const ShaderSourceType shader_source) const
	{
		ShaderSourceSharedPrMap::const_iterator it, end;
		StringVector result;

		end = m_shader_sources.end();

		for (it = m_shader_sources.begin(); it != end; ++it)
		{
			if (it->first.first == shader_source)
			{
				result.push_back(it->first.second);
			}
		}

		return result;
	}

}
