/****************************************************************************
 *            shadersourcebuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourcebuilder.hpp"
#include "exceptions.hpp"
#include "autoparameterutil.hpp"
#include "lua.hpp"
#include "vertexelement.hpp"
#include "logging.hpp"
#include "globalvars.hpp"
#include "shadersourcedescription.hpp"
#include "utf.hpp"
#include "commonparameterutil.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "shadertextureutil.hpp"
#include "filesystem.hpp"

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
			sslt_i
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

				inline ~ShaderSourceLocalTypeData() throw()
				{
				}

				inline const String &get_name() const
				{
					return m_name;
				}

				inline ParameterType get_type() const
				{
					return m_type;
				}

		};

		const ShaderSourceLocalTypeData
			shader_source_local_type_datas[] =
		{
			ShaderSourceLocalTypeData(
				String(UTF8("diffuse_colors_sum")), pt_vec3),
			ShaderSourceLocalTypeData(
				String(UTF8("specular_colors_sum")), pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8("vertex_color")),
				pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8(
				"fragment_color")), pt_vec3),
			ShaderSourceLocalTypeData(String(UTF8("i")), pt_int)
		};

		const Uint32 shader_source_local_type_datas_count =
			sizeof(shader_source_local_type_datas) /
			sizeof(ShaderSourceLocalTypeData);

		const String &get_str(const ShaderSourceLocalType local)
		{
			if (shader_source_local_type_datas_count > local)
			{
				return shader_source_local_type_datas[
					local].get_name();
			}

			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_source_local_type_datas_count
						 - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					local))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderSourceLocalType")));
		}

		ParameterType get_parameter_type(
			const ShaderSourceLocalType local)
		{
			if (shader_source_local_type_datas_count > local)
			{
				return shader_source_local_type_datas[
					local].get_type();
			}

			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_source_local_type_datas_count
						 - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					local))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderSourceLocalType")));
		}

		ShaderSourceParameter build_parameter(
			const ShaderSourceLocalType local,
			const ParameterQualifierType qualifier)
		{
			if (shader_source_local_type_datas_count > local)
			{
				return ShaderSourceParameterBuilder::build(
					shader_source_local_type_datas[
						local].get_name(),
					shader_source_local_type_datas[
						local].get_type(),
					qualifier, 1);
			}

			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_source_local_type_datas_count
						 - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					local))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderSourceLocalType")));
		}

		void add_parameter(const AutoParameterType auto_parameter,
			const ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameter tmp;

			tmp = ShaderSourceParameterBuilder::build(
				auto_parameter);

			ShaderSourceParameterBuilder::add_parameter(tmp, locals,
				parameters);
		}

		void add_parameter(const CommonParameterType common_parameter,
			const ParameterQualifierType qualifier,
			const ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameter tmp;

			tmp = ShaderSourceParameterBuilder::build(
				common_parameter, qualifier);

			ShaderSourceParameterBuilder::add_parameter(tmp, locals,
				parameters);
		}

		void add_parameter(const ShaderSourceLocalType local,
			const ParameterQualifierType qualifier,
			const ShaderSourceParameterVector &locals,
			ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameter tmp;

			tmp = build_parameter(local, qualifier);

			ShaderSourceParameterBuilder::add_parameter(tmp, locals,
				parameters);
		}

		void add_local(const CommonParameterType common_parameter,
			const ParameterQualifierType qualifier,
			ShaderSourceParameterVector &locals,
			const ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameter tmp;

			tmp = ShaderSourceParameterBuilder::build(
				common_parameter, qualifier);

			ShaderSourceParameterBuilder::add_local(tmp, locals,
				parameters);
		}

		void add_local(const ShaderSourceLocalType local,
			const ParameterQualifierType qualifier,
			ShaderSourceParameterVector &locals,
			const ShaderSourceParameterVector &parameters)
		{
			ShaderSourceParameter tmp;

			tmp = build_parameter(local, qualifier);

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

				i++;
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
			Uint32 i;

			i = 0;

			while (i < inputs.size())
			{
				it = inputs.begin() + i;

				if (VertexElement::get_is_vertex_semantic_type(
					it->get_name()))
				{
					attributes.push_back(*it);
					inputs.erase(it);
				}
				else
				{
					i++;
				}
			}
		}

		void build_varyings(
			ShaderSourceParameterVector &vertex_parameters,
			ShaderSourceParameterVector &fragment_parameters,
			ShaderSourceParameterVector &varyings)
		{
			ShaderSourceParameterVector::iterator it;
			StringSet names;
			Uint32 i;

			i = 0;

			while (i < vertex_parameters.size())
			{
				it = vertex_parameters.begin() + i;

				if (it->get_qualifier() == pqt_out)
				{
					names.insert(it->get_name());
					varyings.push_back(*it);
					vertex_parameters.erase(it);
				}
				else
				{
					i++;
				}
			}

			i = 0;

			while (i < fragment_parameters.size())
			{
				it = fragment_parameters.begin() + i;

				if (names.count(it->get_name()) > 0)
				{
					fragment_parameters.erase(it);
				}
				else
				{
					i++;
				}
			}
		}

		void write_parameter(
			const ShaderSourceParameter &parameter,
			const ParameterSizeTypeUint16Map &array_sizes,
			const StringType &prefix, OutStream &str)
		{
			str << prefix;
			parameter.write(array_sizes, str);
			str << UTF8(";\n");
		}

		void write_parameters(
			const ShaderSourceParameterVector &parameters,
			const ParameterSizeTypeUint16Map &array_sizes,
			const StringType &prefix, OutStream &str)
		{
			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				parameters)
			{
				write_parameter(parameter, array_sizes, prefix,
					str);
			}
		}

		void write_parameters(const ParameterQualifierType qualifier,
			const ShaderSourceParameterVector &parameters,
			const ParameterSizeTypeUint16Map &array_sizes,
			const StringType &prefix, OutStream &str)
		{
			BOOST_FOREACH(const ShaderSourceParameter &parameter,
				parameters)
			{
				if (parameter.get_qualifier() == qualifier)
				{
					write_parameter(parameter, array_sizes,
						prefix, str);
				}
			}
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
				parameter.write_parameter(array_sizes, str,
					first);
			}

			str << UTF8(")\n");
			str << UTF8("{\n");
			write_parameters(locals, array_sizes,
				UTF8("\t"), str);
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
				parameter.write_name(str, first);
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
					const StringType &shader_source);

				~OptimizeShaderSource() throw();

				StringType get_source();

		};

		OptimizeShaderSource::OptimizeShaderSource(
			glslopt_ctx* optimizer, glslopt_shader_type type,
			const StringType &source): m_optimizer(optimizer),
			m_shader(0)
		{
			m_shader = glslopt_optimize(m_optimizer, type,
				string_to_utf8(source).c_str(), 0);
		}

		OptimizeShaderSource::~OptimizeShaderSource() throw()
		{
			glslopt_shader_delete(m_shader);
		}

		StringType OptimizeShaderSource::get_source()
		{
			if (glslopt_get_status(m_shader))
			{
				return utf8_to_string(glslopt_get_output(
					m_shader));
			}
			else
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(utf8_to_string(
						glslopt_get_log(m_shader))));
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

	}

	enum ShaderSourceBuildOptionTypes
	{
		ssbot_tangent = 0,
		ssbot_transparent,
		ssbot_view_direction,
		ssbot_world_uv,
		ssbot_fragment_uv,
		ssbot_view_position,
		ssbot_alpha_to_coverage
	};

	class ShaderSourceBuilder::ShaderSourceBuildData
	{
		private:
			ShaderSourceTypeStringMap m_sources;
			ShaderSourceDataType m_type;
			ShaderBuildType m_shader_build_type;
			BitSet16 m_options;
			Uint16 m_vertex_light_count;
			Uint16 m_fragment_light_count;
			Uint16 m_light_count;

		public:
			inline ShaderSourceBuildData(): m_type(ssdt_glsl_120),
				m_shader_build_type(sbt_color),
				m_vertex_light_count(0),
				m_fragment_light_count(0), m_light_count(0)
			{
			}

			inline ShaderSourceBuildData(
				const ShaderSourceTypeStringMap &sources,
				const ShaderSourceDataType type,
				const ShaderBuildType shader_build_type,
				const Uint16 vertex_light_count,
				const Uint16 fragment_light_count,
				const Uint16 light_count): m_sources(sources),
				m_type(type),
				m_shader_build_type(shader_build_type),
				m_vertex_light_count(vertex_light_count),
				m_fragment_light_count(fragment_light_count),
				m_light_count(light_count)
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
			inline ~ShaderSourceBuildData() throw()
			{
			}

			/**
			 * Returns the options the shader uses.
			 */
			inline void set_option(
				const ShaderSourceBuildOptionTypes option,
				const bool enabled)
			{
				m_options[option] = enabled;
			}

			/**
			 * Returns the sources of the different shader parts.
			 */
			inline const ShaderSourceTypeStringMap &get_sources()
				const
			{
				return m_sources;
			}

			/**
			 * Returns the shader build type.
			 */
			inline ShaderBuildType get_shader_build_type() const
			{
				return m_shader_build_type;
			}

			inline ShaderSourceDataType get_type() const
			{
				return m_type;
			}

			/**
			 * Returns the options the shader uses.
			 */
			inline bool get_option(
				const ShaderSourceBuildOptionTypes option) const
			{
				return m_options[option];
			}

			/**
			 * Returns the options the shader uses.
			 */
			inline BitSet16 get_options() const
			{
				return m_options;
			}

			/**
			 * Returns the number of lights the shader uses.
			 */
			inline Uint16 get_light_count() const
			{
				return m_light_count;
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

	};

	ShaderSourceBuilder::ShaderSourceBuilder(
		const GlobalVarsSharedPtr &global_vars,
		const FileSystemWeakPtr &file_system):
		m_global_vars(global_vars), m_file_system(file_system),
		m_optimizer(0)
	{
		assert(m_global_vars.get() != 0);
		assert(!m_file_system.expired());

		m_optimizer = glslopt_initialize(false, true);

		m_shadow_scale = 0.6f;
		m_vertex_light_count = 4;
		m_fragment_light_count = 4;
		m_bone_count = 72;
		m_dynamic_light_count = false;
	}

	ShaderSourceBuilder::~ShaderSourceBuilder() throw()
	{
		glslopt_cleanup(m_optimizer);
	}

	void ShaderSourceBuilder::load_file(const String &file_name)
	{
		std::auto_ptr<ShaderSource> shader_source;
		ShaderSourceTypeStringPair index;

		try
		{
			shader_source.reset(new ShaderSource());

			shader_source->load_xml(file_name);

			index.first = shader_source->get_type();
			index.second = shader_source->get_name();

			m_shader_sources.insert(index, shader_source);
		}
		catch (boost::exception &exception)
		{
			exception << boost::errinfo_file_name(string_to_utf8(
				file_name));

			LOG_EXCEPTION(exception);
		}
	}

	void ShaderSourceBuilder::load(const String &file_name)
	{
		Lua lua;
		String file;
		ShaderSource shader_source;

		lua.do_string(get_file_system()->get_file_string(file_name),
			file_name);

		/**
		 * Fetch the string from the Lua context and make sure
		 * it exists:
		 */
		lua.get_global("files");

		if (!lua.is_table(-1))
		{
			lua.pop(1);
			return;
		}

		lua.push_nil();

		while (lua.next(-2))
		{
			if (lua.is_string(-1))
			{
				file = lua.to_string(-1);
				load_file(file);
			}

			lua.pop(1);
		}

		lua.pop(1);
	}

	void ShaderSourceBuilder::load_default(const String &file_name)
	{
		Lua lua;
		String tmp;
		Uint32 i, count;
		ShaderSourceType shader_source_type;

		lua.do_string(get_file_system()->get_file_string(file_name),
			file_name);

		m_defaults.clear();

		// Fetch the table from the Lua context and make sure it exists:
		lua.get_global("defaults");

		if (!lua.is_table(-1))
		{
			lua.pop(1);

			return;
		}

		count = ShaderSourceUtil::get_shader_source_count();

		for (i = 0; i < count; i++)
		{
			shader_source_type =
				static_cast<ShaderSourceType>(i);

			tmp = ShaderSourceUtil::get_str(shader_source_type);

			lua.push_string(tmp);
			lua.get_table(-2);

			if (lua.is_string(-1))
			{
				m_defaults[shader_source_type] =
					lua.to_string(-1);
			}

			lua.pop(1);
		}
		lua.pop(2);
	}

	bool ShaderSourceBuilder::build_function(
		const ShaderSourceBuildData &data,
		const ParameterSizeTypeUint16Map &array_sizes,
		const ShaderSourceParameterVector &locals, 
		const ShaderSourceType shader_source_type,
		OutStream &functions, OutStream &local, 
		ShaderSourceParameterVector &globals,
		StringVariantMap &values) const
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
			LOG_ERROR(UTF8("Can't find '%1%' for source type %2%."),
				index->second % shader_source_type);
			return false;
		}

		found->second->build_function(data.get_type(), array_sizes,
			locals, functions, local, globals);

		return true;
	}

	void ShaderSourceBuilder::build_lights(
		const ShaderSourceBuildData &data,
		const ParameterSizeTypeUint16Map &array_sizes,
		const ShaderSourceParameterVector &locals, 
		const bool vertex, const bool shadow, OutStream &functions,
		OutStream &main, ShaderSourceParameterVector &globals,
		StringVariantMap &values) const
	{
		ShaderSourceParameterVector function_locals;
		ShaderSourceParameterVector function_parameters;
		ShaderSourceLocalType output_color;
		CommonParameterType input_normal;
		StringStream function;
		StringType lighting;
		Uint16 light_offset, light_count;
		bool dynamic_light_count;

		lighting = UTF8("lighting");

		if (vertex)
		{
			output_color = sslt_vertex_color;
			input_normal = cpt_world_normal;
			light_offset = get_fragment_light_count();
			light_count = data.get_light_count();
			dynamic_light_count = true;
		}
		else
		{
			input_normal = cpt_fragment_normal;
			output_color = sslt_fragment_color;
			light_offset = 0;
			light_count = data.get_fragment_light_count();
			dynamic_light_count =
				get_dynamic_light_count();
		}

		add_local(sslt_i, pqt_out, function_locals,
			function_parameters);
		add_local(cpt_lighting_normal, pqt_in, function_locals,
			function_parameters);
		add_parameter(input_normal, pqt_in, function_locals,
			function_parameters);
		add_parameter(cpt_world_position, pqt_in, function_locals,
			function_parameters);

		function << UTF8("\t") << cpt_lighting_normal;
		function << UTF8(" = ") << input_normal << UTF8(";\n");

		add_local(sslt_diffuse_colors_sum, pqt_out, function_locals,
			function_parameters);

		function << UTF8("\t") << sslt_diffuse_colors_sum;
		function << UTF8(" = ");

		if (vertex || (data.get_vertex_light_count() == 0))
		{
			add_parameter(apt_ambient, function_locals,
				function_parameters);

			function << apt_ambient << UTF8(".rgb;\n");
		}
		else
		{
			add_parameter(sslt_vertex_color, pqt_in,
				function_locals, function_parameters);

			function << sslt_vertex_color << UTF8(";\n");
		}

		if (!vertex)
		{
			add_local(sslt_specular_colors_sum, pqt_out,
				function_locals, function_parameters);

			function << UTF8("\t");
			function << sslt_specular_colors_sum;
			function << UTF8(" = vec3(0.0);\n");
		}

		function << UTF8("\n\tfor (i = ") << light_offset;
		function << UTF8("; i < ");

		if (dynamic_light_count)
		{
			add_parameter(apt_dynamic_light_count, function_locals,
				function_parameters);

			function << apt_dynamic_light_count;

			if (vertex)
			{
				function << UTF8(".z");
			}
			else
			{
				function << UTF8(".y");
			}
		}
		else
		{
			function << light_count;
		}

		function << UTF8("; i++)\n\t{\n");

		add_local(cpt_light_color, pqt_in, function_locals,
			function_parameters);
		add_parameter(apt_light_colors, function_locals,
			function_parameters);

		function << UTF8("\t\t") << cpt_light_color;
		function << UTF8(" = ") << apt_light_colors;
		function << UTF8("[i];\n");

		add_local(cpt_light_position, pqt_in, function_locals,
			function_parameters);
		add_parameter(apt_light_positions, function_locals,
			function_parameters);

		function << UTF8("\t\t") << cpt_light_position;
		function << UTF8(" = ") << apt_light_positions;
		function << UTF8("[i];\n\t");

		if (!build_function(data, array_sizes, function_locals,
			sst_light, functions, function, function_parameters,
			values))
		{
			add_parameter(cpt_diffuse, pqt_in, locals, globals);

			add_parameter(output_color, pqt_out, locals, globals);

			main << UTF8("\t") << output_color << UTF8(" = ");
			main << cpt_diffuse << UTF8(".rgb;\n");

			return;
		}

		function << UTF8("\n");

		make_parameter_local(
			CommonParameterUtil::get_str(cpt_diffuse_color),
			function_locals, function_parameters);
		make_parameter_local(
			CommonParameterUtil::get_str(cpt_specular_color),
			function_locals, function_parameters);

		add_local(cpt_diffuse_color, pqt_in, function_locals,
			function_parameters);

		if (!vertex && shadow)
		{
			add_parameter(cpt_shadow, pqt_in, function_locals,
				function_parameters);

			function << UTF8("\t\tif (i == 0)\n\t\t{\n");
			function << UTF8("\t\t\t") << cpt_diffuse_color;
			function << UTF8(" *= ") << cpt_shadow << UTF8(" * ");
			function << get_shadow_scale() << UTF8(" + ");
			function << (1.0 - get_shadow_scale());
			function << UTF8(";\n");
			function << UTF8("\t\t\t") << cpt_specular_color;
			function << UTF8(" *= ") << cpt_shadow << UTF8(";\n");
			function << UTF8("\t\t}\n");
		}

		function << UTF8("\t\t") << sslt_diffuse_colors_sum;
		function << UTF8(" += ") << cpt_diffuse_color << UTF8(";\n");

		if (!vertex)
		{
			add_local(cpt_specular_color, pqt_in,
				function_locals, function_parameters);

			function << UTF8("\t\t") << sslt_specular_colors_sum;
			function << UTF8(" += ") << cpt_specular_color;
			function << UTF8(";\n");
		}

		function << UTF8("\t}\n\n");

		add_parameter(output_color, pqt_out, function_locals,
			function_parameters);

		function << UTF8("\t") << output_color << UTF8(" = ");
		function << sslt_diffuse_colors_sum;

		if (!vertex)
		{
			add_parameter(cpt_diffuse, pqt_in, function_locals,
				function_parameters);

			function << UTF8(" * ") << cpt_diffuse << UTF8(".rgb");
		}

		function << UTF8(";\n");

		if (!vertex)
		{
			add_parameter(cpt_specular, pqt_in, function_locals,
				function_parameters);

			function << UTF8("\t") << output_color << UTF8(" += ");
			function << cpt_specular << UTF8(" * ");
			function << sslt_specular_colors_sum << UTF8(";\n");
		}

		write_function(lighting, function.str(), function_locals,
			function_parameters, array_sizes, functions);

		write_function_use(lighting, UTF8("\t"), function_parameters,
			main);

		add_parameters(function_parameters, locals, globals);
	}

	void ShaderSourceBuilder::build_vertex_source(
		const ShaderSourceBuildData &data,
		const ParameterSizeTypeUint16Map &array_sizes,
		OutStream &functions, OutStream &main,
		ShaderSourceParameterVector &globals,
		StringVariantMap &values) const
	{
		ShaderSourceParameterVector locals;
		StringType gl_Position;

		gl_Position = UTF8("gl_Position");

		if (data.get_option(ssbot_tangent))
		{
			build_function(data, array_sizes, locals,
				sst_world_tangent_transform, functions, main,
				globals, values);
		}
		else
		{
			if (data.get_shader_build_type() != sbt_color)
			{
				build_function(data, array_sizes,
					locals, sst_world_depth_transform,
					functions, main, globals, values);
			}
			else
			{
				build_function(data, array_sizes,
					locals, sst_world_normal_transform,
					functions, main, globals, values);
			}
		}

		add_parameter(apt_projection_view_matrix, locals, globals);
		add_parameter(cpt_world_position, pqt_in, locals, globals);

		main << UTF8("\t") << gl_Position;
		main << UTF8(" = ") << apt_projection_view_matrix;
		main << UTF8(" * vec4(") << cpt_world_position;
		main << UTF8(", 1.0);\n");

		if (data.get_option(ssbot_view_position))
		{
			build_function(data, array_sizes, locals,
				sst_view_transform, functions, main, globals,
				values);
		}

		if (data.get_shader_build_type() == sbt_color)
		{
			if (get_fog())
			{
				build_function(data, array_sizes, locals,
					sst_fog, functions, main, globals,
					values);
			}

			if (data.get_vertex_light_count() > 0)
			{
				build_lights(data, array_sizes, locals,
					true, false, functions, main, globals,
					values);
			}

			if ((get_shadow_map_count() > 0) &&
				(data.get_fragment_light_count() > 0))
			{
				build_function(data, array_sizes, locals,
					sst_shadow_uv, functions, main, globals,
					values);
			}
		}

		if (data.get_option(ssbot_world_uv))
		{
			build_function(data, array_sizes, locals, sst_uv,
				functions, main, globals, values);
		}

		main << UTF8("}\n");
	}

	void ShaderSourceBuilder::build_fragment_source(
		const ShaderSourceBuildData &data,
		const ParameterSizeTypeUint16Map &array_sizes,
		OutStream &functions, OutStream &main,
		ShaderSourceParameterVector &globals,
		StringVariantMap &values) const
	{
		ShaderSourceParameterVector locals;
		StringType gl_FragColor;
		bool shadows;

		gl_FragColor = UTF8("gl_FragColor");

		shadows = false;

		if (data.get_option(ssbot_view_direction))
		{
			build_function(data, array_sizes, locals,
				sst_view_direction, functions, main, globals,
				values);
		}

		if ((data.get_shader_build_type() != sbt_color) ||
			(data.get_fragment_light_count() == 0))
		{
			if (data.get_option(ssbot_fragment_uv))
			{
				build_function(data, array_sizes,
					locals, sst_normal_depth_mapping,
					functions, main, globals, values);
			}
		}
		else
		{
			build_function(data, array_sizes, locals,
				sst_normal_mapping, functions, main, globals,
				values);
		}

		if ((data.get_shader_build_type() == sbt_color) ||
			data.get_option(ssbot_transparent))
		{
			build_function(data, array_sizes, locals,
				sst_diffuse_mapping, functions, main, globals,
				values);

			if (!data.get_option(ssbot_alpha_to_coverage))
			{
				build_function(data, array_sizes, locals,
					sst_transparent, functions, main,
					globals, values);
			}
		}

		if (data.get_shader_build_type() == sbt_color)
		{
			build_function(data, array_sizes, locals,
				sst_specular_mapping, functions, main, globals,
				values);

			if (data.get_fragment_light_count() > 0)
			{
				if (get_shadow_map_count() > 0)
				{
					shadows = build_function(data,
						array_sizes, locals,
						sst_shadow_mapping, functions,
						main, globals, values);
				}

				build_lights(data, array_sizes, locals, false,
					shadows, functions, main, globals,
					values);
			}
			else
			{
				add_parameter(sslt_fragment_color, pqt_out,
					locals, globals);

				main << UTF8("\t") << sslt_fragment_color;
				main << UTF8(" = ") << cpt_diffuse;
				main << UTF8(".rgb * ");

				if (data.get_vertex_light_count() > 0)
				{
					add_parameter(sslt_vertex_color, pqt_in,
						locals, globals);
					main << sslt_vertex_color;
					main << UTF8(";\n");
				}
				else
				{
					add_parameter(apt_ambient, locals,
						globals);
					main << apt_ambient << UTF8(".rgb;\n");
				}
			}
		}

		switch (data.get_shader_build_type())
		{
			case sbt_color:
				add_parameter(sslt_fragment_color, pqt_in,
					locals, globals);

				main << UTF8("\t") << gl_FragColor;
				main << UTF8(".rgb = ");

				if (get_fog())
				{
					add_parameter(cpt_fog, pqt_in, locals,
						globals);
					add_parameter(apt_fog_data, locals,
						globals);

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

				break;
			case sbt_depth:
				main << UTF8("\t") << gl_FragColor;
				main << UTF8(".rgb = vec3(1.0)");

				break;
			case sbt_shadow:
				build_function(data, array_sizes, locals,
					sst_shadow_map, functions, main,
					globals, values);

				add_parameter(cpt_shadow_map_data, pqt_in,
					locals, globals);

				main << UTF8("\t") << gl_FragColor;
				main << UTF8(".rgb = ");
				main << cpt_shadow_map_data;

				break;
		}

		main << UTF8(";\n\tgl_FragColor.a = ");

		if (data.get_option(ssbot_transparent) &&
			data.get_option(ssbot_alpha_to_coverage))
		{
			main << cpt_diffuse << UTF8(".a");
		}
		else
		{
			main << UTF8("1.0");
		}

		main << UTF8(";\n}\n");
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
			return false;
		}

		return found->second->check_source_parameter(data.get_type(),
			name);
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
				sst_view_transform);
		}

		if ((data.get_shader_build_type() == sbt_color))
		{
			if (get_fog())
			{
				result |= check_function(data, name, sst_fog);
			}

			if (data.get_vertex_light_count() > 0)
			{
				result |= check_function(data, name, sst_light);
			}

			if ((get_shadow_map_count() > 0) &&
				(data.get_fragment_light_count() > 0))
			{
				result |= check_function(data, name,
					sst_shadow_uv);
			}
		}

		if (data.get_option(ssbot_world_uv))
		{
			result |= check_function(data, name, sst_uv);
		}

		if (data.get_option(ssbot_view_direction))
		{
			result |= check_function(data, name,
				sst_view_direction);
		}

		if ((data.get_shader_build_type() != sbt_color) ||
			(data.get_fragment_light_count() == 0))
		{
			if (data.get_option(ssbot_fragment_uv))
			{
				result |= check_function(data, name,
					sst_normal_depth_mapping);
			}
		}
		else
		{
			result |= check_function(data, name,
				sst_normal_mapping);
		}

		if ((data.get_shader_build_type() == sbt_color) ||
			data.get_option(ssbot_transparent))
		{
			result |= check_function(data, name,
				sst_diffuse_mapping);
			result |= check_function(data, name,
				sst_transparent);
		}

		if (data.get_shader_build_type() == sbt_color)
		{
			result |= check_function(data, name,
				sst_specular_mapping);

			if (data.get_fragment_light_count() > 0)
			{
				if (get_shadow_map_count() > 0)
				{
					result |= check_function(data,
						name, sst_shadow_mapping);
				}

				result |= check_function(data, name,
					sst_light);
			}
		}

		if (data.get_shader_build_type() == sbt_shadow)
		{
			result |= check_function(data, name, sst_shadow_map);
		}

		return result;
	}

	bool ShaderSourceBuilder::get_transparent(
		const ShaderSourceTypeStringMap &types) const
	{
		ShaderSourceTypeStringMap::const_iterator found;

		found = types.find(sst_transparent);

		if (found == types.end())
		{
			return false;
		}

		return m_shader_sources.find(*found) != m_shader_sources.end();
	}

	void ShaderSourceBuilder::set_shadow_map_type(const String &name)
	{
		m_defaults[sst_shadow_mapping] = name;
		m_defaults[sst_shadow_map] = name;
	}

	const String &ShaderSourceBuilder::get_quality() const
	{
		return m_global_vars->get_quality();
	}

	Uint16 ShaderSourceBuilder::get_shadow_map_count() const
	{
		return m_global_vars->get_shadow_map_count();
	}

	bool ShaderSourceBuilder::get_fog() const
	{
		return m_global_vars->get_fog();
	}

	void ShaderSourceBuilder::build(
		const ShaderSourceDescription &description, StringType &vertex,
		StringType &fragment, StringVariantMap &values) const
	{
		ShaderSourceParameterVector attributes, varyings;
		ShaderSourceParameterVector vertex_globals;
		ShaderSourceParameterVector fragment_globals;
		ParameterSizeTypeUint16Map array_sizes;
		StringSet vertex_extensions, fragment_extensions;
		StringStream vertex_functions, vertex_main;
		StringStream fragment_functions, fragment_main;
		StringStream vertex_source, fragment_source;
		Uint32 i, count;
		Uint16 version;
		ShaderSourceBuildData data;

		data = ShaderSourceBuildData(description.get_sources(),
			ssdt_glsl_120, description.get_shader_build_type(),
			get_vertex_light_count(), get_fragment_light_count(),
			std::min(get_light_count(),
				description.get_light_count()));

		vertex = UTF8("");
		fragment = UTF8("");
		values.clear();

		version = 130;

		array_sizes[pst_light_count] = data.get_light_count();
		array_sizes[pst_bone_count] = get_bone_count();
		array_sizes[pst_shadow_map_count] =
			get_shadow_map_count();

		data.set_option(ssbot_transparent,
			get_transparent(description.get_sources()));
		data.set_option(ssbot_view_direction, get_source_parameter(data,
			cpt_world_view_direction));
		data.set_option(ssbot_fragment_uv, get_source_parameter(data,
			cpt_fragment_uv));
		data.set_option(ssbot_world_uv, get_source_parameter(data,
			cpt_world_uv));
		data.set_option(ssbot_view_position, get_source_parameter(data,
			cpt_view_position));
		data.set_option(ssbot_tangent, get_source_parameter(data,
			cpt_world_tangent));

		if (data.get_shader_build_type() == sbt_color)
		{
			data.set_option(ssbot_alpha_to_coverage,
				data.get_option(ssbot_transparent) &&
				get_global_vars()->get_alpha_to_coverage());
		}

		if (data.get_shader_build_type() == sbt_shadow)
		{
			data.set_option(ssbot_alpha_to_coverage,
				data.get_option(ssbot_transparent) &&
				get_global_vars()->get_msaa_shadows());
		}

		build_fragment_source(data, array_sizes, fragment_functions,
			fragment_main, fragment_globals, values);
		build_vertex_source(data, array_sizes, vertex_functions,
			vertex_main, vertex_globals, values);

		build_varyings(vertex_globals, fragment_globals, varyings);
		build_attributes(vertex_globals, attributes);

		vertex_source << UTF8("#version ") << version;
		vertex_source << UTF8("\n");
		BOOST_FOREACH(const String &extension, vertex_extensions)
		{
			vertex_source << UTF8("#extension ");
			vertex_source << extension;
			vertex_source << UTF8(" : enable\n");
		}

		vertex = vertex_source.str();

		build_constants(array_sizes, vertex_source);
		vertex_source << vertex_functions.str();
		vertex_source << UTF8("\n");
		write_parameters(pqt_in, attributes, array_sizes,
			UTF8("attribute "), vertex_source);
		vertex_source << UTF8("\n");
		write_parameters(pqt_in, vertex_globals, array_sizes,
			UTF8("uniform "), vertex_source);
		vertex_source << UTF8("\n");
		write_parameters(varyings, array_sizes,
			UTF8("varying "), vertex_source);
		vertex_source << UTF8("\n");
		vertex_source << UTF8("void main()\n");
		vertex_source << UTF8("{\n");
		write_parameters(pqt_out, vertex_globals, array_sizes,
			UTF8("\t"), vertex_source);
		vertex_source << UTF8("\n");
		vertex_source << vertex_main.str();

		fragment_source << UTF8("#version ") << version;
		fragment_source << UTF8("\n");
		BOOST_FOREACH(const String &extension, fragment_extensions)
		{
			fragment_source << UTF8("#extension ");
			fragment_source << extension;
			fragment_source << UTF8(" : enable\n");
		}

		fragment = fragment_source.str();

		build_constants(array_sizes, fragment_source);
		fragment_source << fragment_functions.str();
		fragment_source << UTF8("\n");
		write_parameters(pqt_in, fragment_globals, array_sizes,
			UTF8("uniform "), fragment_source);
		fragment_source << UTF8("\n");
		write_parameters(varyings, array_sizes,
			UTF8("varying "), fragment_source);
		fragment_source << UTF8("\n");
		fragment_source << UTF8("void main()\n");
		fragment_source << UTF8("{\n");
		write_parameters(pqt_out, fragment_globals, array_sizes,
			UTF8("\t"), fragment_source);
		fragment_source << UTF8("\n");
		fragment_source << fragment_main.str();

		LOG_DEBUG(UTF8("Vertex Shader:\n%1%"), vertex_source.str());
		LOG_DEBUG(UTF8("Fragment Shader:\n%1%"), fragment_source.str());

		try
		{
			vertex = get_optimized_source(m_optimizer, vertex,
				kGlslOptShaderVertex, vertex_source.str());
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_vertex_shader_source(
				vertex_source.str());

			throw;
		}

		try
		{
			fragment = get_optimized_source(m_optimizer, fragment,
				kGlslOptShaderFragment, fragment_source.str());
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_fragment_shader_source(
				fragment_source.str());

			throw;
		}

		count = ShaderTextureUtil::get_shader_texture_count();

		for (i = 0; i < count; i++)
		{
			values[ShaderTextureUtil::get_str(
				static_cast<ShaderTextureType>(i))] =
					static_cast<Sint64>(i);
		}	
	}

}
