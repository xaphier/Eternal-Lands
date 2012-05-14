/****************************************************************************
 *            shadersourcebuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourcebuilder.hpp"
#include "exceptions.hpp"
#include "autoparameterutil.hpp"
#include "vertexelement.hpp"
#include "logging.hpp"
#include "globalvars.hpp"
#include "effectdescription.hpp"
#include "commonparameterutil.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "shadertextureutil.hpp"
#include "filesystem.hpp"
#include "glsl_optimizer/glsl/glsl_optimizer.h"
#include "xmlreader.hpp"
#include "effect.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "abstractterrainmanager.hpp"
#include "glmutil.hpp"
#include "glslprogramdescription.hpp"

namespace eternal_lands
{

	namespace
	{

		enum ShaderSourceLocalType
		{
			sslt_diffuse_colors_sum,
			sslt_specular_colors_sum,
			sslt_vertex_color,
			sslt_fragment_color,
			sslt_shadow_values,
			sslt_layer_index,
			sslt_layer,
			sslt_i,
			sslt_position
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
			ShaderSourceLocalTypeData(String(UTF8("vertex_color")),
				pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8(
				"fragment_color")), pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8("shadow_values")),
				pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8("layer_index")),
				pt_int),
			ShaderSourceLocalTypeData(String(UTF8("layer_data")),
				pt_int),
			ShaderSourceLocalTypeData(String(UTF8("i")), pt_int),
			ShaderSourceLocalTypeData(String(UTF8("position")),
				pt_vec3)
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
					qualifier, 1);
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
			ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameter tmp;

			tmp = ShaderSourceParameterBuilder::build(source,
				auto_parameter);

			ShaderSourceParameterBuilder::add_parameter(tmp, locals,
				parameters);
		}

		void add_parameter(const String &source,
			const CommonParameterType common_parameter,
			const ParameterQualifierType qualifier,
			const ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameter tmp;

			tmp = ShaderSourceParameterBuilder::build(source,
				common_parameter, qualifier);

			ShaderSourceParameterBuilder::add_parameter(tmp, locals,
				parameters);
		}

		void add_parameter(const String &source,
			const ShaderSourceLocalType local,
			const ParameterQualifierType qualifier,
			const ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameter tmp;

			tmp = build_parameter(source, local, qualifier);

			ShaderSourceParameterBuilder::add_parameter(tmp, locals,
				parameters);
		}

		void add_local(const String &source,
			const CommonParameterType common_parameter,
			const ParameterQualifierType qualifier,
			ShaderSourceParameterVector &locals,
			const ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameter tmp;

			tmp = ShaderSourceParameterBuilder::build(source,
				common_parameter, qualifier);

			ShaderSourceParameterBuilder::add_local(tmp, locals,
				parameters);
		}

		void add_local(const String &source,
			const ShaderSourceLocalType local,
			const ParameterQualifierType qualifier,
			ShaderSourceParameterVector &locals,
			const ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameter tmp;

			tmp = build_parameter(source, local, qualifier);

			ShaderSourceParameterBuilder::add_local(tmp, locals,
				parameters);
		}

		OutStream& operator<<(OutStream &str,
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
			ShaderSourceParameterVector &globals)
		{
			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				function_parameters)
			{
				ShaderSourceParameterBuilder::add_parameter(
					parameter, locals, globals);
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
			const StringType &prefix, const StringType &suffix,
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
			const StringType &prefix, const StringType &suffix,
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
			const StringType &prefix, const StringType &suffix,
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
			const StringType &prefix, const bool use_block,
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
			const StringType &prefix, const bool use_block,
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
			const StringType &prefix, const String &index,
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
			const StringType &prefix, const bool use_block,
			OutStream &stream)
		{
			stream << prefix << name_prefix;

			if (use_block)
			{
				stream << UTF8(".");
			}

			stream << parameter.get_name();
		}

		void write_function(const StringType &name,
			const StringType &function,
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
				UTF8("\t"), UTF8(""), str);
			str << UTF8("\n");
			str << function;
			str << UTF8("}\n\n");
		}

		void write_function_use(const StringType &name,
			const StringType &prefix,
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

			str << UTF8("const vec3 terrain_vector_scale = vec3(");
			str << AbstractTerrainManager::get_vector_scale().x;
			str << ", ";
			str << AbstractTerrainManager::get_vector_scale().y;
			str << ", ";
			str << AbstractTerrainManager::get_vector_scale().z;
			str << UTF8(");\n");
		}

		class OptimizeShaderSource
		{
			private:
				glslopt_ctx* m_optimizer;
				glslopt_shader* m_shader;

			public:
				OptimizeShaderSource(glslopt_ctx* optimizer,
					glslopt_shader_type type,
					const StringType &shader_source);

				~OptimizeShaderSource() noexcept;

				StringType get_source();

		};

		OptimizeShaderSource::OptimizeShaderSource(
			glslopt_ctx* optimizer, glslopt_shader_type type,
			const StringType &source): m_optimizer(optimizer),
			m_shader(0)
		{
			m_shader = glslopt_optimize(m_optimizer, type,
				source.c_str(), 0);
		}

		OptimizeShaderSource::~OptimizeShaderSource() noexcept
		{
			glslopt_shader_delete(m_shader);
		}

		StringType OptimizeShaderSource::get_source()
		{
			if (glslopt_get_status(m_shader))
			{
				return glslopt_get_output(m_shader);
			}
			else
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(glslopt_get_log(
						m_shader)));
			}
		}

		String get_optimized_source(glslopt_ctx* optimizer,
			const StringType &prefix,
			const glslopt_shader_type type,
			const StringType &source)
		{
			OptimizeShaderSource oss(optimizer, type, source);
			StringType str;

			str = prefix;
			str += oss.get_source();

			return String(str);
		}

		ShaderVersionType get_shader_version_type(const Uint16 version)
		{
			if (version >= 420)
			{
				return svt_420;
			}

			if (version >= 410)
			{
				return svt_410;
			}

			if (version >= 400)
			{
				return svt_400;
			}

			if (version >= 330)
			{
				return svt_330;
			}

			if (version >= 150)
			{
				return svt_150;
			}

			if (version >= 140)
			{
				return svt_140;
			}

			if (version >= 130)
			{
				return svt_130;
			}

			return svt_120;
		}

		void write_defines(const StringType &prefix,
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
						array_sizes, UTF8("in "),
						UTF8(""), stream);
				}
			}
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
		ssbot_world_uv,
		ssbot_world_extra_uv,
		ssbot_fragment_uv,
		ssbot_view_position,
		ssbot_tbn_matrix,
		ssbot_fog,
		ssbot_alpha_test,
		ssbot_use_functions,
		ssbot_lighting
	};

	class ShaderSourceBuilder::ShaderSourceBuildData
	{
		private:
			ShaderSourceTypeStringMap m_sources;
			ShaderVersionType m_version;
			ShaderBuildType m_shader_build_type;
			std::set<ShaderSourceBuildOptionTypes> m_options;
			Uint16 m_vertex_light_count;
			Uint16 m_fragment_light_count;
			Uint16 m_light_count;
			Uint16 m_shadow_map_count;
			Uint16 m_layer_count;

		public:
			inline ShaderSourceBuildData():	m_version(svt_120),
				m_shader_build_type(sbt_default),
				m_vertex_light_count(0),
				m_fragment_light_count(0), m_light_count(0),
				m_shadow_map_count(0), m_layer_count(0)
			{
			}

			inline ShaderSourceBuildData(
				const ShaderSourceTypeStringMap &sources,
				const ShaderVersionType version,
				const ShaderBuildType shader_build_type,
				const Uint16 vertex_light_count,
				const Uint16 fragment_light_count,
				const Uint16 light_count,
				const Uint16 shadow_map_count,
				const Uint16 layer_count):
				m_sources(sources), m_version(version),
				m_shader_build_type(shader_build_type),
				m_vertex_light_count(vertex_light_count),
				m_fragment_light_count(fragment_light_count),
				m_light_count(light_count),
				m_shadow_map_count(shadow_map_count),
				m_layer_count(layer_count)
			{
				Sint32 tmp;

				tmp = light_count - fragment_light_count;

				m_vertex_light_count = std::min(
					vertex_light_count,
					static_cast<Uint16>(std::max(0, tmp)));
				m_fragment_light_count = std::min(light_count,
					fragment_light_count);
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
			inline ShaderBuildType get_shader_build_type() const
				noexcept
			{
				return m_shader_build_type;
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
			inline Uint16 get_light_count() const noexcept
			{
				return m_light_count;
			}

			/**
			 * Returns the number of vertex lights the shader uses.
			 */
			inline Uint16 get_vertex_light_count() const noexcept
			{
				return m_vertex_light_count;
			}

			/**
			 * Returns the number of fragment lights the shader
			 * uses.
			 */
			inline Uint16 get_fragment_light_count() const noexcept
			{
				return m_fragment_light_count;
			}

			/**
			 * Returns the number of shadow maps the shader uses.
			 */
			inline Uint16 get_shadow_map_count() const noexcept
			{
				return m_shadow_map_count;
			}

			/**
			 * Returns the number of layer the shader uses.
			 */
			inline Uint16 get_layer_count() const noexcept
			{
				return m_layer_count;
			}

	};

	ShaderSourceBuilder::ShaderSourceBuilder(
		const GlobalVarsSharedPtr &global_vars):
		m_global_vars(global_vars)
	{
		assert(m_global_vars.get() != nullptr);

		m_optimizer.reset(new ShaderSourceOptimizer());

		m_shadow_scale = 0.8f;
		m_vertex_light_count = 4;
		m_fragment_light_count = 4;
		m_bone_count = 72;
		m_dynamic_light_count = false;
	}

	ShaderSourceBuilder::~ShaderSourceBuilder() noexcept
	{
	}

	void ShaderSourceBuilder::load_shader_source(
		const FileSystemSharedPtr &file_system, const String &file_name)
	{
		std::auto_ptr<ShaderSource> shader_source;
		ShaderSourceTypeStringPair index;

		try
		{
			shader_source.reset(new ShaderSource());

			shader_source->load_xml(file_system, file_name);

			index.first = shader_source->get_type();
			index.second = shader_source->get_name();

			m_shader_sources.insert(index, shader_source);

			LOG_INFO(lt_shader_source, UTF8("Shader source type "
				"%1%-%2% loaded from file '%3%'"), index.first
				% index.second % file_name);
		}
		catch (boost::exception &exception)
		{
			exception << boost::errinfo_file_name(file_name);

			LOG_EXCEPTION(exception);
		}
	}

	void ShaderSourceBuilder::load_shader_sources(
		const FileSystemSharedPtr &file_system, const xmlNodePtr node)
	{
		xmlNodePtr it;
		String name, file_name;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("shader_sources"))
			!= 0)
		{
			return;
		}

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("shader_source"))
				== 0)
			{
				load_shader_source(file_system,
					XmlUtil::get_string_value(it));
			}
		}
		while (XmlUtil::next(it, true));
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
					m_sources[type] =
						XmlUtil::get_string_value(it);

					break;
				}
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceBuilder::load_xml(
		const FileSystemSharedPtr &file_system, const xmlNodePtr node)
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
		m_vertex_light_count = 4;
		m_fragment_light_count = 4;
		m_bone_count = 72;
		m_instance_count = 1;
		m_dynamic_light_count = false;

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("shadow_scale"))
				== 0)
			{
				m_shadow_scale = XmlUtil::get_float_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("vertex_light_count")) == 0)
			{
				m_vertex_light_count =
					XmlUtil::get_uint16_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("fragment_light_count")) == 0)
			{
				m_fragment_light_count =
					XmlUtil::get_uint16_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("bone_count"))
				== 0)
			{
				m_bone_count = XmlUtil::get_uint16_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("instance_count")) == 0)
			{
				m_instance_count =
					XmlUtil::get_uint16_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("dynamic_light_count")) == 0)
			{
				m_dynamic_light_count =
					XmlUtil::get_bool_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("sources"))
				== 0)
			{
				load_sources(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("shader_sources"))
				== 0)
			{
				load_shader_sources(file_system, it);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceBuilder::load_xml(
		const FileSystemSharedPtr &file_system, const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(file_system, xml_reader->get_root_node());
	}

	bool ShaderSourceBuilder::build_function(
		const ShaderSourceBuildData &data,
		const ParameterSizeTypeUint16Map &array_sizes,
		const ShaderSourceParameterVector &locals,
		const ShaderSourceType shader_source_type,
		const String &indent, OutStream &stream, OutStream &functions,
		ShaderSourceParameterVector &globals) const
	{
		ShaderSourceTypeStringPairShaderSourceMap::const_iterator found;
		ShaderSourceTypeStringMap::const_iterator index;

		index = data.get_sources().find(shader_source_type);

		if (index == data.get_sources().end())
		{
			return false;
		}

		found = m_shader_sources.find(*index);

		if (found == m_shader_sources.end())
		{
			LOG_ERROR(lt_shader_source, UTF8("Shader source type "
				"not found %1%-%2%"), index->first %
				index->second);
			return false;
		}

		/**
		 * AMD driver doesn't like functions with array arguments.
		 */
		if (data.get_option(ssbot_use_functions) &&
			!GLEW_AMD_name_gen_delete)
		{
			found->second->build_function(data.get_version(),
				locals, array_sizes, String(UTF8("")),
				String(UTF8("")), indent, stream, functions,
				globals);
		}
		else
		{
			found->second->build_source(data.get_version(), locals,
				indent, stream, globals);
		}

		return true;
	}

	void ShaderSourceBuilder::build_lights(
		const ShaderSourceBuildData &data,
		const ParameterSizeTypeUint16Map &array_sizes,
		const ShaderSourceParameterVector &locals,
		const String &indent, const bool vertex, const bool shadow,
		OutStream &main, OutStream &functions,
		ShaderSourceParameterVector &globals) const
	{
		ShaderSourceParameterVector function_locals;
		ShaderSourceParameterVector function_parameters;
		ShaderSourceLocalType output_color;
		ShaderSourceType light;
		StringStream stream;
		String local_indent, local_loop_indent;
		StringType lighting;
		Uint16 light_offset, light_count;
		bool dynamic_light_count;

		lighting = UTF8("lighting");
		local_indent = indent.get() + UTF8('\t');
		local_loop_indent = local_indent.get() + UTF8('\t');

		if (vertex)
		{
			output_color = sslt_vertex_color;
			light_offset = get_fragment_light_count();
			light_count = data.get_light_count();
			dynamic_light_count = true;
			light = sst_vertex_light;
		}
		else
		{
			output_color = sslt_fragment_color;
			light_offset = 0;
			light_count = data.get_fragment_light_count();
			dynamic_light_count =
				get_dynamic_light_count();
			light = sst_fragment_light;
		}

		add_local(String(UTF8("lighting")), sslt_i, pqt_out,
			function_locals, function_parameters);
		add_local(String(UTF8("lighting")), sslt_shadow_values,
			pqt_out, function_locals, function_parameters);

		stream << UTF8("\n");

		if (data.get_shader_build_type() == sbt_debug_diffuse_light)
		{
			add_parameter(String(UTF8("lighting")),
				sslt_diffuse_colors_sum, pqt_out,
				function_locals, function_parameters);
		}
		else
		{
			add_local(String(UTF8("lighting")),
				sslt_diffuse_colors_sum, pqt_out,
				function_locals, function_parameters);
		}

		stream << local_indent << sslt_diffuse_colors_sum;
		stream << UTF8(" = ");

		if (vertex || (data.get_vertex_light_count() == 0))
		{
			add_parameter(String(UTF8("lighting")), apt_ambient,
				function_locals, function_parameters);

			stream << apt_ambient << UTF8(".rgb;\n");
		}
		else
		{
			add_parameter(String(UTF8("lighting")),
				sslt_vertex_color, pqt_in, function_locals,
				function_parameters);

			stream << sslt_vertex_color << UTF8(";\n");
		}

		if (!vertex)
		{
			add_local(String(UTF8("lighting")), cpt_emission,
				pqt_in, function_locals, function_parameters);


			if (data.get_shader_build_type() ==
				sbt_debug_specular_light)
			{
				add_parameter(String(UTF8("lighting")),
					sslt_specular_colors_sum, pqt_out,
					function_locals, function_parameters);
			}
			else
			{
				add_local(String(UTF8("lighting")),
					sslt_specular_colors_sum, pqt_out,
					function_locals, function_parameters);
			}

			stream << local_indent << sslt_diffuse_colors_sum;
			stream << UTF8(" += ") << cpt_emission << UTF8(";\n");

			stream << local_indent << sslt_specular_colors_sum;
			stream << UTF8(" = vec3(0.0);\n");
		}

		if (!vertex && shadow)
		{
			add_parameter(String(UTF8("lighting")), cpt_shadow,
				pqt_in, function_locals, function_parameters);

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

		if (dynamic_light_count && !vertex)
		{
			add_parameter(String(UTF8("lighting")),
				apt_dynamic_light_count, function_locals,
				function_parameters);

			stream << apt_dynamic_light_count;
		}
		else
		{
			stream << light_count;
		}

		stream << UTF8("; ++") << sslt_i << UTF8(")\n");
		stream << local_indent << UTF8("{\n");

		add_local(String(UTF8("lighting")), cpt_light_color, pqt_in,
			function_locals, function_parameters);
		add_parameter(String(UTF8("lighting")), apt_light_colors,
			function_locals, function_parameters);

		stream << local_loop_indent << cpt_light_color;
		stream << UTF8(" = ") << apt_light_colors;
		stream << UTF8("[") << sslt_i << UTF8("];\n");

		add_local(String(UTF8("lighting")), cpt_light_position, pqt_in,
			function_locals, function_parameters);
		add_parameter(String(UTF8("lighting")), apt_light_positions,
			function_locals, function_parameters);

		stream << local_loop_indent << cpt_light_position;
		stream << UTF8(" = ") << apt_light_positions;
		stream << UTF8("[") << sslt_i << UTF8("];\n");

		build_function(data, array_sizes, function_locals,
			light, local_loop_indent, stream, functions,
			function_parameters);

		make_parameter_local(
			CommonParameterUtil::get_str(cpt_diffuse_color),
			function_locals, function_parameters);
		make_parameter_local(
			CommonParameterUtil::get_str(cpt_specular_color),
			function_locals, function_parameters);

		add_local(String(UTF8("lighting")), cpt_diffuse_color, pqt_in,
			function_locals, function_parameters);

		stream << local_loop_indent << sslt_diffuse_colors_sum;
		stream << UTF8(" += ") << cpt_diffuse_color;

		if (!vertex && shadow)
		{
			add_parameter(String(UTF8("lighting")), cpt_shadow,
				pqt_in, function_locals, function_parameters);

			stream << UTF8(" * ") << sslt_shadow_values;
			stream << UTF8(".x");
		}

		stream << UTF8(";\n");

		if (!vertex)
		{
			add_local(String(UTF8("lighting")), cpt_specular_color,
				pqt_in, function_locals, function_parameters);

			stream << local_loop_indent << sslt_specular_colors_sum;
			stream << UTF8(" += ") << cpt_specular_color;

			if (shadow)
			{
				stream << UTF8(" * ") << sslt_shadow_values;
				stream << UTF8(".y");
			}

			stream << UTF8(";\n");
		}

		if (!vertex && shadow)
		{
			stream << local_loop_indent << sslt_shadow_values;
			stream << UTF8(".xy = ") << sslt_shadow_values;
			stream << UTF8(".zz;\n");
		}

		stream << local_indent << UTF8("}\n\n");

		add_parameter(String(UTF8("lighting")), output_color, pqt_out,
			function_locals, function_parameters);

		stream << local_indent << output_color << UTF8(" = ");
		stream << sslt_diffuse_colors_sum;

		if (!vertex)
		{
			add_parameter(String(UTF8("lighting")), cpt_albedo,
				pqt_in, function_locals, function_parameters);

			stream << UTF8(" * ") << cpt_albedo << UTF8(".rgb");
		}

		stream << UTF8(";\n");

		if (!vertex)
		{
			add_parameter(String(UTF8("lighting")), cpt_specular,
				pqt_in, function_locals, function_parameters);

			stream << local_indent << output_color << UTF8(" += ");
			stream << cpt_specular << UTF8(" * ");
			stream << sslt_specular_colors_sum << UTF8(";\n");
		}

		main << indent << UTF8("/* lighting */\n");
		main << indent << UTF8("{\n");

		write_parameters(String(), function_locals, array_sizes,
			local_indent, UTF8(""), main);

		main << stream.str() << indent << UTF8("}\n");

		add_parameters(function_parameters, locals, globals);
	}

	void ShaderSourceBuilder::build_vertex_source(
		const ShaderSourceBuildData &data,
		const ParameterSizeTypeUint16Map &array_sizes,
		OutStream &main, OutStream &functions,
		ShaderSourceParameterVector &globals) const
	{
		ShaderSourceParameterVector locals;
		String gl_Position, indent;

		indent = UTF8("\t");
		gl_Position = UTF8("gl_Position");

		if (data.get_option(ssbot_tangent))
		{
			build_function(data, array_sizes, locals,
				sst_world_tangent_transformation, indent, main,
				functions, globals);
		}
		else
		{
			if (data.get_option(ssbot_normal))
			{
				build_function(data, array_sizes,
					locals, sst_world_normal_transformation,
					indent, main, functions,
					globals);
			}
			else
			{
				build_function(data, array_sizes,
					locals, sst_world_depth_transformation,
					indent, main, functions,
					globals);
			}
		}

		add_parameter(String(UTF8("vertex")),
			apt_projection_view_matrices, locals, globals);
		add_parameter(String(UTF8("vertex")), cpt_world_position,
			pqt_in, locals, globals);

		main << indent << UTF8("/* gl_Position */\n");
		main << indent << gl_Position;
		main << UTF8(" = ") << apt_projection_view_matrices;
		main << UTF8("[0] * vec4(") << cpt_world_position;
		main << UTF8(", 1.0);\n");

		if (data.get_option(ssbot_view_position))
		{
			build_function(data, array_sizes, locals,
				sst_view_transformation, indent, main,
				functions, globals);
		}

		if ((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index) ||
			(data.get_shader_build_type() == sbt_debug_shadow) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light))
		{
			if (data.get_option(ssbot_fog))
			{
				build_function(data, array_sizes, locals,
					sst_fog, indent, main, functions,
					globals);
			}

			if ((data.get_vertex_light_count() > 0) &&
				data.get_option(ssbot_lighting))
			{
				build_lights(data, array_sizes, locals, indent,
					true, false, main, functions, globals);
			}

			if ((data.get_shadow_map_count() > 0) &&
				(data.get_fragment_light_count() > 0))
			{
				build_function(data, array_sizes, locals,
					sst_shadow_uv, indent, main, functions,
					globals);
			}
		}

		if (data.get_option(ssbot_world_uv) ||
			data.get_option(ssbot_world_extra_uv))
		{
			build_function(data, array_sizes, locals, sst_uv,
				indent, main, functions, globals);
		}
	}

	void ShaderSourceBuilder::build_geometry_source(
		const ShaderSourceBuildData &data,
		const ParameterSizeTypeUint16Map &array_sizes,
		const ShaderSourceParameterVector &varyings,
		const String &in_prefix, const String &out_prefix,
		const bool use_block, OutStream &main, OutStream &functions,
		ShaderSourceParameterVector &globals) const
	{
		ShaderSourceParameterVector locals;
		String gl_Position, index, indent;
		Uint16 i, j, count, size;

		gl_Position = UTF8("gl_Position");

		add_parameter(String(UTF8("geometry")), sslt_i, pqt_out,
			locals, globals);
		add_parameter(String(UTF8("geometry")), cpt_world_position,
			pqt_in, locals, globals);
		add_parameter(String(UTF8("geometry")),
			apt_projection_view_matrices, locals, globals);

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
			main << apt_projection_view_matrices << UTF8("[");
			main << sslt_i << UTF8("] * vec4(");

			write_parameter_indexed(in_prefix,
				ShaderSourceParameterBuilder::build(
					String(UTF8("geometry")),
					cpt_world_position, pqt_in),
				UTF8(""), index, use_block, main);

			main << UTF8(", 1.0);\n");

			main << indent << UTF8("/* copying vertex output to ");
			main << UTF8("fragment input */\n");

			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				varyings)
			{
				if ((parameter.get_size() == pst_one) &&
					(parameter.get_scale() == 1))
				{
					write_parameter(out_prefix, parameter,
						indent, use_block, main);

					write_parameter_indexed(in_prefix,
						parameter, UTF8(" = "),
						index, use_block, main);

					main << UTF8(";\n");

					continue;
				}

				size = parameter.get_array_size(array_sizes);

				for (j = 0; j < size; ++j)
				{
					StringStream str;

					str << UTF8("[") << j << UTF8("]");

					write_parameter(out_prefix, parameter,
						indent, use_block, main);

					main << str.str();

					write_parameter_indexed(in_prefix,
						parameter, UTF8(" = "), index,
						use_block, main);

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
	}

	void ShaderSourceBuilder::build_fragment_source(
		const ShaderSourceBuildData &data,
		const ParameterSizeTypeUint16Map &array_sizes,
		OutStream &main, OutStream &functions,
		ShaderSourceParameterVector &globals) const
	{
		ShaderSourceParameterVector locals;
		StringArray8 output_array;
		String output, indent;
		Uint16 i, count;
		bool shadows;

		indent = UTF8("\t");

		if (data.get_version() > svt_150)
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

			if (data.get_version() > svt_150)
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

		if (data.get_option(ssbot_view_direction))
		{
			build_function(data, array_sizes, locals,
				sst_view_direction, indent, main, functions,
				globals);
		}

		if (data.get_option(ssbot_tbn_matrix))
		{
			build_function(data, array_sizes, locals,
				sst_tbn_matrix, indent, main, functions,
				globals);
		}

		if ((((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index) ||
			(data.get_shader_build_type() == sbt_debug_shadow) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light)) &&
			(data.get_fragment_light_count() > 0) &&
			data.get_option(ssbot_lighting)) ||
			(data.get_shader_build_type() == sbt_debug_normal))
		{
			build_function(data, array_sizes, locals,
				sst_normal_mapping, indent, main, functions,
				globals);
		}
		else
		{
			if (data.get_option(ssbot_fragment_uv))
			{
				build_function(data, array_sizes,
					locals, sst_normal_depth_mapping,
					indent, main, functions, globals);
			}
		}

		if ((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index) ||
			(data.get_shader_build_type() == sbt_debug_alpha) ||
			(data.get_shader_build_type() == sbt_debug_albedo) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light) ||
			data.get_option(ssbot_transparent))
		{
			build_function(data, array_sizes, locals,
				sst_albedo_mapping, indent, main, functions,
				globals);

			if (data.get_option(ssbot_transparent) &&
				data.get_option(ssbot_alpha_test))
			{
				add_parameter(String(UTF8("fragment")),
					cpt_albedo, pqt_in, locals, globals);

				main << indent << UTF8("if (") << cpt_albedo;
				main << UTF8(".a < 0.5)\n") << indent;
				main << UTF8("{\n") << indent;
				main << UTF8("\tdiscard;\n") << indent;
				main << UTF8("}\n");
			}
		}

		if ((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index) ||
			(data.get_shader_build_type() == sbt_debug_emissive) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light))
		{
			build_function(data, array_sizes, locals,
				sst_emission_mapping, indent, main, functions,
				globals);
		}

		if ((((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light)) &&
			(data.get_fragment_light_count() > 0) &&
			data.get_option(ssbot_lighting)) ||
			(data.get_shader_build_type() == sbt_debug_specular))
		{
			build_function(data, array_sizes, locals,
				sst_specular_mapping, indent, main, functions,
				globals);
		}

		if ((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index) ||
			(data.get_shader_build_type() == sbt_debug_shadow) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light))
		{
			if ((data.get_fragment_light_count() > 0) &&
				data.get_option(ssbot_lighting))
			{
				if (data.get_shadow_map_count() > 0)
				{
					shadows = build_function(data,
						array_sizes, locals,
						sst_shadow_mapping, indent,
						main, functions, globals);
				}

				build_lights(data, array_sizes, locals, indent, 
					false, shadows, main, functions,
					globals);
			}
			else
			{
				add_parameter(String(UTF8("fragment")),
					sslt_fragment_color, pqt_out, locals,
					globals);
				add_parameter(String(UTF8("fragment")),
					cpt_albedo, pqt_in, locals, globals);

				main << indent << sslt_fragment_color;
				main << UTF8(" = ") << cpt_albedo;
				main << UTF8(".rgb * (");

				if ((data.get_vertex_light_count() > 0) &&
					data.get_option(ssbot_lighting))
				{
					add_parameter(String(UTF8("fragment")),
						sslt_vertex_color, pqt_in,
						locals, globals);
					main << sslt_vertex_color;
				}
				else
				{
					add_parameter(String(UTF8("fragment")),
						apt_ambient, locals, globals);

					main << apt_ambient << UTF8(".rgb");
				}

				add_parameter(String(UTF8("fragment")),
					cpt_emission, pqt_in, locals, globals);

				main << UTF8(" + ") << cpt_emission;
				main << UTF8(");\n");
			}
		}

		switch (data.get_shader_build_type())
		{
			case sbt_default:
			case sbt_light_index:
				add_parameter(String(UTF8("fragment")),
					sslt_fragment_color, pqt_in, locals,
					globals);

				main << indent << output << UTF8(".rgb = ");

				if (data.get_option(ssbot_fog))
				{
					add_parameter(String(UTF8("fragment")),
						cpt_fog, pqt_in, locals,
						globals);
					add_parameter(String(UTF8("fragment")),
						apt_fog_data, locals, globals);

					main << UTF8("mix(") << apt_fog_data;
					main << UTF8(".rgb, ");
					main << sslt_fragment_color;
					main << UTF8(", ");
					main << cpt_fog << UTF8(")");
				}
				else
				{
					main << sslt_fragment_color;
				}

				main << UTF8(";\n");

				break;
			case sbt_depth:
				main << indent << output;
				main << UTF8(" = vec4(1.0);\n");

				break;
			case sbt_shadow:
				build_function(data, array_sizes, locals,
					sst_shadow_map, indent, main,
					functions, globals);

				add_parameter(String(UTF8("fragment")),
					cpt_shadow_map_data, pqt_in, locals,
					globals);

				main << indent << output << UTF8(".rgb = ");
				main << cpt_shadow_map_data << UTF8(";\n");

				break;
			case sbt_debug_uv:
				add_parameter(String(UTF8("fragment")),
					cpt_world_uv, pqt_in, locals,
					globals);
				main << indent << output << UTF8(".rgb = ");
				main << UTF8("vec3(") << cpt_world_uv;
				main << UTF8(", 0.0);\n");
				break;
			case sbt_debug_depth:
				add_parameter(String(UTF8("fragment")),
					apt_z_params, locals, globals);
				main << indent << output << UTF8(".rgb = ");
				main << UTF8("vec3(2.0 * z_params.x / ");
				main << UTF8("(z_params.z - (2.0 * ");
				main << UTF8("gl_FragCoord.z - 1.0) * ");
				main << UTF8("z_params.w));\n");
				break;
			case sbt_debug_alpha:
				add_parameter(String(UTF8("fragment")),
					cpt_emission, pqt_in, locals,
					globals);
				main << indent << output << UTF8(".rgb = ");
				main << cpt_albedo << UTF8(".aaa;\n");
				break;
			case sbt_debug_albedo:
				add_parameter(String(UTF8("fragment")),
					cpt_emission, pqt_in, locals,
					globals);
				main << indent << output << UTF8(".rgb = ");
				main << cpt_albedo << UTF8(".rgb;\n");
				break;
			case sbt_debug_normal:
				add_parameter(String(UTF8("fragment")),
					cpt_fragment_normal, pqt_in, locals,
					globals);
				main << indent << output << UTF8(".rgb = ");
				main << cpt_fragment_normal;
				main << UTF8(" * 0.5 + 0.5;\n");
				break;
			case sbt_debug_shadow:
				main << indent << output << UTF8(".rgb = ");

				if ((data.get_fragment_light_count() == 0) ||
					(data.get_shadow_map_count() == 0) ||
					!data.get_option(ssbot_lighting))
				{
					main << UTF8("vec3(1.0);\n");
					break;
				}

				add_parameter(String(UTF8("fragment")),
					cpt_shadow, pqt_in, locals, globals);
				main << UTF8("vec3(") << cpt_shadow;
				main << UTF8(");\n");
				break;
			case sbt_debug_specular:
				add_parameter(String(UTF8("fragment")),
					cpt_specular, pqt_in, locals,
					globals);
				main << indent << output << UTF8(".rgb = ");
				main << UTF8("vec3(") << cpt_specular;
				main << UTF8(");\n");
				break;
			case sbt_debug_emissive:
				add_parameter(String(UTF8("fragment")),
					cpt_emission, pqt_in, locals,
					globals);
				main << indent << output << UTF8(".rgb = ");
				main << cpt_emission << UTF8(";\n");
				break;
			case sbt_debug_diffuse_light:
				main << indent << output << UTF8(".rgb = ");

				if ((data.get_fragment_light_count() == 0) ||
					!data.get_option(ssbot_lighting))
				{
					main << UTF8("vec3(0.0);\n");
					break;
				}

				add_parameter(String(UTF8("fragment")),
					sslt_diffuse_colors_sum, pqt_in,
					locals, globals);
				main << sslt_diffuse_colors_sum << UTF8(";\n");
				break;
			case sbt_debug_specular_light:
				main << indent << output << UTF8(".rgb = ");

				if ((data.get_fragment_light_count() == 0) ||
					!data.get_option(ssbot_lighting))
				{
					main << UTF8("vec3(0.0);\n");
					break;
				}

				add_parameter(String(UTF8("fragment")),
					sslt_specular_colors_sum, pqt_in,
					locals, globals);
				main << sslt_specular_colors_sum << UTF8(";\n");
				break;
		}

		if (data.get_shader_build_type() != sbt_depth)
		{
			main << indent << output << UTF8(".a = ");

			if (data.get_option(ssbot_transparent) &&
				!data.get_option(ssbot_alpha_test))
			{
				add_parameter(String(UTF8("fragment")),
					cpt_albedo, pqt_in, locals,
					globals);

				main << cpt_albedo << UTF8(".a");
			}
			else
			{
				main << UTF8("1.0");
			}

			main << UTF8(";\n");
		}
	}

	bool ShaderSourceBuilder::check_function(
		const ShaderSourceBuildData &data, const String &name,
		const ShaderSourceType shader_source_type) const
	{
		ShaderSourceTypeStringPairShaderSourceMap::const_iterator found;
		ShaderSourceTypeStringMap::const_iterator index;

		index = data.get_sources().find(shader_source_type);

		if (index == data.get_sources().end())
		{
			return false;
		}

		found = m_shader_sources.find(*index);

		if (found == m_shader_sources.end())
		{
			LOG_ERROR(lt_shader_source, UTF8("Shader source type "
				"not found %1%-%2%"), index->first %
				index->second);
			return false;
		}

		return found->second->check_source_parameter(data.get_version(),
			name);
	}

	bool ShaderSourceBuilder::check(
		const ShaderSourceTypeStringPair &source,
		const ShaderVersionType version) const
	{
		ShaderSourceTypeStringPairShaderSourceMap::const_iterator found;

		found = m_shader_sources.find(source);

		if (found == m_shader_sources.end())
		{
			return false;
		}

		return found->second->get_has_data(version);
	}

	bool ShaderSourceBuilder::get_source_parameter(
		const ShaderSourceBuildData &data,
		const CommonParameterType common_parameter) const
	{
		String name;
		bool result;

		name = CommonParameterUtil::get_str(common_parameter);
		result = false;

		if (data.get_option(ssbot_view_position))
		{
			result |= check_function(data, name,
				sst_view_transformation);
		}

		if ((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index) ||
			(data.get_shader_build_type() == sbt_debug_shadow) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light))
		{
			if (data.get_option(ssbot_fog))
			{
				result |= check_function(data, name, sst_fog);
			}

			if (data.get_vertex_light_count() > 0)
			{
				result |= check_function(data, name,
					sst_vertex_light);
			}

			if ((data.get_shadow_map_count() > 0) &&
				(data.get_fragment_light_count() > 0))
			{
				result |= check_function(data, name,
					sst_shadow_uv);
			}
		}

		if (data.get_option(ssbot_world_uv) ||
			data.get_option(ssbot_world_extra_uv))
		{
			result |= check_function(data, name, sst_uv);
		}

		if ((data.get_shader_build_type() == sbt_debug_uv) &&
			((common_parameter == cpt_world_uv) ||
				(common_parameter == cpt_world_extra_uv)))
		{
			result = true;
		}

		if (data.get_option(ssbot_view_direction))
		{
			result |= check_function(data, name,
				sst_view_direction);
		}

		if (data.get_option(ssbot_tbn_matrix))
		{
			result |= check_function(data, name, sst_tbn_matrix);
		}

		if ((((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index)) &&
			(data.get_fragment_light_count() >= 0)) ||
			(data.get_shader_build_type() == sbt_debug_normal) ||
			(data.get_shader_build_type() == sbt_debug_shadow) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light))
		{
			result |= check_function(data, name,
				sst_normal_mapping);
		}
		else
		{
			if (data.get_option(ssbot_fragment_uv))
			{
				result |= check_function(data, name,
					sst_normal_depth_mapping);
			}
		}

		if ((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index) ||
			(data.get_shader_build_type() ==
				sbt_debug_alpha) ||
			(data.get_shader_build_type() ==
				sbt_debug_albedo) ||
			(data.get_shader_build_type() ==
				sbt_debug_shadow) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light) ||
			data.get_option(ssbot_transparent))
		{
			result |= check_function(data, name,
				sst_albedo_mapping);

			if (common_parameter == cpt_albedo)
			{
				result = true;
			}
		}

		if ((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index) ||
			(data.get_shader_build_type() == sbt_debug_shadow) ||
			(data.get_shader_build_type() == sbt_debug_emissive) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light))
		{
			result |= check_function(data, name,
				sst_emission_mapping);

			if (data.get_fragment_light_count() > 0)
			{
				result |= check_function(data, name,
					sst_specular_mapping);
			}
		}


		if ((((data.get_shader_build_type() == sbt_default) ||
			(data.get_shader_build_type() == sbt_light_index)) &&
			(data.get_fragment_light_count() > 0)) ||
			(data.get_shader_build_type() == sbt_debug_shadow) ||
			(data.get_shader_build_type() ==
				sbt_debug_diffuse_light) ||
			(data.get_shader_build_type() ==
				sbt_debug_specular_light))
		{
			if (data.get_shadow_map_count() > 0)
			{
				result |= check_function(data, name,
					sst_shadow_mapping);
			}

			result |= check_function(data, name,
				sst_fragment_light);
		}

		if (data.get_shader_build_type() == sbt_shadow)
		{
			result |= check_function(data, name, sst_shadow_map);
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
		m_sources[sst_shadow_mapping] = name;
		m_sources[sst_shadow_map] = name;
	}

	ShaderSourceTypeStringMap ShaderSourceBuilder::build_sources(
		const EffectDescription &description) const
	{
		ShaderSourceTypeStringMap::iterator found;
		ShaderSourceTypeStringMap sources;

		sources = get_sources();

		sources[sst_world_depth_transformation] =
			description.get_world_transformation();
		sources[sst_world_normal_transformation] =
			description.get_world_transformation();
		sources[sst_world_tangent_transformation] =
			description.get_world_transformation();
		sources[sst_uv] = description.get_texture_coodrinates();
		sources[sst_albedo_mapping] = description.get_albedo_mapping();
		sources[sst_normal_mapping] = description.get_normal_mapping();
		sources[sst_normal_depth_mapping] =
			description.get_normal_mapping();
		sources[sst_specular_mapping] =
			description.get_specular_mapping();
		sources[sst_emission_mapping] =
			description.get_emission_mapping();

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

		if (!description.get_lighting())
		{
			found = sources.find(sst_vertex_light);

			if (found != sources.end())
			{
				sources.erase(found);
			}

			found = sources.find(sst_fragment_light);

			if (found != sources.end())
			{
				sources.erase(found);
			}
		}

		return sources;
	}

	void ShaderSourceBuilder::build(const EffectDescription &description,
		const ShaderBuildType shader_build_type,
		const Uint16 light_count,
		GlslProgramDescription &program_description) const
	{
		ShaderSourceBuildData data;
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
		String vertex_data, fragment_data, prefix, name_prefix;
		String vertex, geometry, fragment;
		Uint16 version, layer_count;
		ShaderVersionType version_type;
		bool use_block, use_alias, use_in_out;

		sources = build_sources(description);

		program_description.set_vertex_shader(String(UTF8("")));
		program_description.set_tess_control_shader(String(UTF8("")));
		program_description.set_tess_evaluation_shader(String(
			UTF8("")));
		program_description.set_geometry_shader(String(UTF8("")));
		program_description.set_fragment_shader(String(UTF8("")));

		if (get_global_vars()->get_optmize_shader_source())
		{
			version = 120;
		}
		else
		{
			version = get_global_vars()->get_glsl_version();
		}

		version_type = get_shader_version_type(version);

		version_stream << UTF8("#version ") << version << UTF8("\n");

		if (shader_build_type == sbt_shadow)
		{
			layer_count = std::max(static_cast<Uint16>(1),
				get_global_vars()->get_shadow_map_count());
		}
		else
		{
			layer_count = static_cast<Uint16>(1);
		}

		data = ShaderSourceBuildData(sources, version_type,
			shader_build_type, get_vertex_light_count(),
			get_fragment_light_count(),
			std::min(get_light_count(), light_count),
			get_global_vars()->get_shadow_map_count(),
			layer_count);

		use_block = (data.get_version() >= svt_150) &&
			get_global_vars()->get_use_block();
		use_alias = get_global_vars()->get_use_alias();
		use_in_out = (data.get_version() >= svt_130) &&
			get_global_vars()->get_use_in_out();

		array_sizes[pst_light_count] = data.get_light_count();
		array_sizes[pst_bone_count] = get_bone_count();
		array_sizes[pst_shadow_map_count] =
			data.get_shadow_map_count();
		array_sizes[pst_layer_count] = layer_count;
		array_sizes[pst_instance_count] = get_instance_count();
		array_sizes[pst_clipmap_slices] =
			get_global_vars()->get_clipmap_slices();

		data.set_option(ssbot_transparent,
			description.get_transparent());
		data.set_option(ssbot_view_direction, get_source_parameter(data,
			cpt_world_view_direction));
		data.set_option(ssbot_fragment_uv, get_source_parameter(data,
			cpt_fragment_uv));
		data.set_option(ssbot_world_uv, get_source_parameter(data,
			cpt_world_uv));
		data.set_option(ssbot_world_extra_uv, get_source_parameter(data,
			cpt_world_extra_uv));
		data.set_option(ssbot_tbn_matrix, get_source_parameter(data,
			cpt_tbn_matrix));
		data.set_option(ssbot_view_position, get_source_parameter(data,
			cpt_view_position));
		data.set_option(ssbot_normal, get_source_parameter(data,
			cpt_world_normal));
		data.set_option(ssbot_tangent, get_source_parameter(data,
			cpt_world_tangent));
		data.set_option(ssbot_transparent,
			description.get_transparent());
		data.set_option(ssbot_alpha_test,
			!(get_global_vars()->get_exponential_shadow_maps() &&
			(shader_build_type == sbt_shadow)));
		data.set_option(ssbot_use_functions,
			get_global_vars()->get_use_functions());
		data.set_option(ssbot_lighting, description.get_lighting());

		build_fragment_source(data, array_sizes, fragment_main,
			fragment_functions, fragment_globals);
		build_vertex_source(data, array_sizes, vertex_main,
			vertex_functions, vertex_globals);

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
		vertex_source << UTF8("invariant gl_Position; /* make ");
		vertex_source << UTF8("existing gl_Position be invariant*/\n");

		vertex = String(vertex_source.str());

		build_constants(array_sizes, vertex_source);
		vertex_source << UTF8("\n");

		vertex_source << UTF8("/* functions */\n");
		vertex_source << vertex_functions.str();
		vertex_source << UTF8("\n");

		vertex_source << UTF8("/* vertex shader input */\n");

		if (data.get_version() > svt_150)
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
				prefix, UTF8(""), vertex_source);
		}

		vertex_source << UTF8("\n");
		vertex_source << UTF8("/* uniforms */\n");
		write_parameters(String(), pqt_in, vertex_globals, array_sizes,
			UTF8("uniform "), UTF8(""), vertex_source);

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
					array_sizes, UTF8("\t"), UTF8(""),
					vertex_source);
				write_parameters(String(),
					geometry_in_parameters, array_sizes,
					UTF8("\t"), UTF8(""), vertex_source);
				vertex_source << UTF8("}");

				if (use_alias)
				{
					vertex_source << UTF8(" ");
					vertex_source << vertex_data;
				}

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

			if (use_alias)
			{
				name_prefix = vertex_data;
			}
			else
			{
				name_prefix = UTF8("");
			}

			write_parameters(name_prefix, varyings, array_sizes,
				prefix, UTF8(""), vertex_source);
			write_parameters(name_prefix, geometry_in_parameters, 
				array_sizes, prefix, UTF8(""), vertex_source);
		}

		vertex_source << UTF8("\n");
		vertex_source << UTF8("void main()\n");
		vertex_source << UTF8("{\n");
		write_parameters(String(), pqt_out, vertex_globals, array_sizes,
			UTF8("\t"), UTF8(""), vertex_source);

		if (use_alias)
		{
			vertex_source << UTF8("\t/* Aliases */\n");
			write_parameters(String(), varyings, array_sizes,
				UTF8("\t"), UTF8(""), vertex_source);
			write_parameters(String(), geometry_in_parameters,
				array_sizes, UTF8("\t"), UTF8(""),
				vertex_source);
		}

		vertex_source << UTF8("\n");
		vertex_source << vertex_main.str();
		vertex_source << UTF8("\n");

		if (use_alias)
		{
			vertex_source << UTF8("\t/* Assign from aliases */\n");
			write_parameters_assign_out(vertex_data, varyings,
				UTF8("\t"), use_block, vertex_source);
			write_parameters_assign_out(vertex_data,
				geometry_in_parameters, UTF8("\t"), use_block,
				vertex_source);
		}

		vertex_source << UTF8("}\n");

		LOG_DEBUG(lt_shader_source, UTF8("Vertex Shader:\n%1%"),
			vertex_source.str());

		geometry_source << version_stream.str();
		geometry_source << UTF8("layout(triangles) in;\n");
		geometry_source << UTF8("layout(triangle_strip) out;\n");
		geometry_source << UTF8("layout(max_vertices = ");
		geometry_source << (data.get_layer_count() * 3);
		geometry_source << UTF8(") out;\n");

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
					array_sizes, UTF8("\t"), UTF8(""),
					geometry_source);
				write_parameters(String(),
					geometry_in_parameters, array_sizes,
					UTF8("\t"), UTF8(""), geometry_source);
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
				prefix, UTF8("[]"), geometry_source);
			write_parameters(vertex_data, geometry_in_parameters,
				array_sizes, prefix, UTF8("[]"),
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
					array_sizes, UTF8("\t"), UTF8(""),
					geometry_source);
				write_parameters(String(),
					geometry_out_parameters, array_sizes,
					UTF8("\t"), UTF8(""), geometry_source);
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
				prefix, UTF8(""), geometry_source);
			write_parameters(fragment_data, geometry_out_parameters,
				array_sizes, prefix, UTF8(""), geometry_source);
		}

		geometry_source << UTF8("\n");
		write_parameters(String(), pqt_in, geometry_globals,
			array_sizes, UTF8("uniform "), UTF8(""),
			geometry_source);
		geometry_source << UTF8("\n");
		geometry_source << UTF8("\n");
		geometry_source << UTF8("void main()\n");
		geometry_source << UTF8("{\n");
		write_parameters(String(), pqt_out, geometry_globals,
			array_sizes, UTF8("\t"), UTF8(""), geometry_source);
		geometry_source << UTF8("\n");
		geometry_source << geometry_main.str();
		geometry_source << UTF8("}\n");

		fragment_source << version_stream.str();

		fragment = String(fragment_source.str());

		build_constants(array_sizes, fragment_source);
		fragment_source << UTF8("\n");

		fragment_source << UTF8("/* functions */\n");
		fragment_source << fragment_functions.str();
		fragment_source << UTF8("\n");

		write_parameters(String(), pqt_in, fragment_globals,
			array_sizes, UTF8("uniform "), UTF8(""),
			fragment_source);
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
					array_sizes, UTF8("\t"), UTF8(""),
					fragment_source);
				write_parameters(String(),
					geometry_out_parameters, array_sizes,
					UTF8("\t"), UTF8(""), fragment_source);
				fragment_source << UTF8("}");

				if (use_alias)
				{
					fragment_source << UTF8(" ");
					fragment_source << fragment_data;
				}

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

			if (use_alias)
			{
				name_prefix = fragment_data;
			}
			else
			{
				name_prefix = UTF8("");
			}

			write_parameters(name_prefix, varyings, array_sizes,
				prefix, UTF8(""), fragment_source);
			write_parameters(name_prefix, geometry_out_parameters,
				array_sizes, prefix, UTF8(""), fragment_source);
		}

		if (data.get_version() > svt_150)
		{
			fragment_source << UTF8("\n");

/*			if (data.get_shader_build_type() == sbt_deferred)
			{
				for (i = 0; i < 4; ++i)
				{
					fragment_source << UTF8("layout(");
					fragment_source << UTF8("location = ");
					fragment_source << i << UTF8(") out");
					fragment_source << UTF8(" vec4 ");
					fragment_source << UTF8("FragData_");
					fragment_source << i << UTF8(";\n");
				}
			}
			else
*/			{
				fragment_source << UTF8("layout(location = 0");
				fragment_source << UTF8(") out vec4 ");
				fragment_source << UTF8("FragColor;\n");
			}
		}

		fragment_source << UTF8("\n");
		fragment_source << UTF8("void main()\n");
		fragment_source << UTF8("{\n");
		write_parameters(String(), pqt_out, fragment_globals,
			array_sizes, UTF8("\t"), UTF8(""), fragment_source);

		if (use_alias)
		{
			fragment_source << UTF8("\t/* Aliases */\n");
			write_parameters(String(), varyings, array_sizes,
				UTF8("\t"), UTF8(""), fragment_source);
			write_parameters(String(), geometry_out_parameters,
				array_sizes, UTF8("\t"), UTF8(""),
				fragment_source);

			fragment_source << UTF8("\n");

			fragment_source << UTF8("\t/* Assign to aliases */\n");
			write_parameters_assign_in(fragment_data, varyings,
				UTF8("\t"), use_block, fragment_source);
			write_parameters_assign_in(fragment_data,
				geometry_out_parameters, UTF8("\t"), use_block,
				fragment_source);
		}

		fragment_source << UTF8("\n");
		fragment_source << fragment_main.str();
		fragment_source << UTF8("}\n");

		LOG_DEBUG(lt_shader_source, UTF8("Fragment Shader:\n%1%"),
			fragment_source.str());

		geometry = UTF8("");

		if (get_global_vars()->get_optmize_shader_source())
		{
			try
			{
				vertex = get_optimized_source(
					m_optimizer->get_optimizer(), vertex,
					kGlslOptShaderVertex,
					vertex_source.str());
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
					fragment_source.str());
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

		program_description.set_vertex_shader(vertex);
		program_description.set_geometry_shader(geometry);
		program_description.set_fragment_shader(fragment);
	}

	StringVector ShaderSourceBuilder::get_shader_source_names(
		const ShaderSourceType shader_source) const
	{
		ShaderSourceTypeStringPairShaderSourceMap::const_iterator it;
		ShaderSourceTypeStringPairShaderSourceMap::const_iterator end;
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
