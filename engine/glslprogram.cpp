/****************************************************************************
 *	    glslprogram.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "glslprogram.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "shader/parameterutil.hpp"

namespace eternal_lands
{

	namespace
	{

		typedef boost::shared_array<GLchar> GLcharSharedArray;
		typedef boost::scoped_array<GLchar*> GLcharPtrScopedArray;
		typedef boost::shared_array<GLchar> GLcharSharedArray;
		typedef std::vector<GLcharSharedArray> GLcharSharedArrayVector;

		class GlslShaderObject
		{
			private:
				GLuint m_shader;

			public:
				GlslShaderObject(const GLenum shader_type);
				~GlslShaderObject() throw();
				void load(const StringType &source);
				StringType get_shader_log() const;
				bool get_shader_compile_status() const;
				void compile();

				inline GLuint get_shader() const
				{
					return m_shader;
				}

		};

		GlslShaderObject::GlslShaderObject(const GLenum shader_type):
			m_shader(0)
		{
			assert((shader_type == GL_VERTEX_SHADER) ||
				(shader_type == GL_FRAGMENT_SHADER));

			m_shader = glCreateShader(shader_type);

			assert(m_shader != 0);
		}

		GlslShaderObject::~GlslShaderObject() throw()
		{
			assert(m_shader != 0);

			glDeleteShader(m_shader);

			m_shader = 0;
		}

		void GlslShaderObject::load(const StringType &source)
		{
			GLint size;

			const char* buffer = source.c_str();
	
			size = source.length();

			assert(size > 0);

			glShaderSource(get_shader(), 1, &buffer, &size);
		}

		StringType GlslShaderObject::get_shader_log() const
		{
			GLcharSharedArray buffer;
			GLint blen, slen;

			glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &blen);

			if (blen > 0)
			{
				buffer.reset(new GLchar[blen]);

				glGetShaderInfoLog(m_shader, blen, &slen,
					buffer.get());

				return std::string(buffer.get(), slen);
			}
			else
			{
				return UTF8("");
			}
		}

		bool GlslShaderObject::get_shader_compile_status() const
		{
			GLint ret;

			glGetShaderiv(m_shader, GL_COMPILE_STATUS, &ret);

			return ret == GL_TRUE;
		}

		void GlslShaderObject::compile()
		{
			glCompileShader(get_shader());

			if (!get_shader_compile_status())
			{
				EL_THROW_EXCEPTION(OpenGlException()
					<< errinfo_message(get_shader_log()));
			}

			LOG_DEBUG(UTF8("GlslShader compiled successful: %1%"),
				get_shader_log());
		}

		String get_base_name(const StringType &name)
		{
			StringType::size_type found;

			found = name.rfind('[');

			if ((found != std::string::npos) &&
				(name.size() == (name.rfind(']') + 1)))
			{
				return String(name.substr(0, found));
			}
			else
			{
				return String(name);
			}
		}

		void set_uniform(const Uint32 index, const bool value)
		{
			glUniform1i(index, value);
		}

		void set_uniform(const Uint32 index, const glm::bvec2 &value)
		{
			glUniform2i(index, value[0], value[1]);
		}

		void set_uniform(const Uint32 index, const glm::bvec3 &value)
		{
			glUniform3i(index, value[0], value[1], value[2]);
		}

		void set_uniform(const Uint32 index, const glm::bvec4 &value)
		{
			glUniform4i(index, value[0], value[1], value[2],
				value[3]);
		}

		void set_uniform(const Uint32 index, const Sint32 value)
		{
			glUniform1i(index, value);
		}

		void set_uniform(const Uint32 index, const glm::ivec2 &value)
		{
			glUniform2iv(index, 1, glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::ivec3 &value)
		{
			glUniform3iv(index, 1, glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::ivec4 &value)
		{
			glUniform4iv(index, 1, glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Sint32Vector &value)
		{
			glUniform1iv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				&value[0]);
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Ivec2Vector &value)
		{
			glUniform2iv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Ivec3Vector &value)
		{
			glUniform3iv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Ivec4Vector &value)
		{
			glUniform4iv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 value)
		{
			glUniform1ui(index, value);
		}

		void set_uniform(const Uint32 index, const glm::uvec2 &value)
		{
			glUniform2uiv(index, 1, glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::uvec3 &value)
		{
			glUniform3uiv(index, 1, glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::uvec4 &value)
		{
			glUniform4uiv(index, 1, glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32Vector &value)
		{
			glUniform1uiv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				&value[0]);
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Uvec2Vector &value)
		{
			glUniform2uiv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Uvec3Vector &value)
		{
			glUniform3uiv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Uvec4Vector &value)
		{
			glUniform4uiv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const float value)
		{
			glUniform1f(index, value);
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const FloatVector &value)
		{
			glUniform1fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				&value[0]);
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Vec2Vector &value)
		{
			glUniform2fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Vec3Vector &value)
		{
			glUniform3fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Vec4Vector &value)
		{
			glUniform4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const glm::vec2 &value)
		{
			glUniform2fv(index, 1, glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::vec3 &value)
		{
			glUniform3fv(index, 1, glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::vec4 &value)
		{
			glUniform4fv(index, 1, glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Mat2x2Vector &value, const GLboolean transpose)
		{
			glUniformMatrix2fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Mat2x3Vector &value, const GLboolean transpose)
		{
			glUniformMatrix2x3fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Mat2x4Vector &value, const GLboolean transpose)
		{
			glUniformMatrix2x4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Mat3x2Vector &value, const GLboolean transpose)
		{
			glUniformMatrix3x2fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Mat3x3Vector &value, const GLboolean transpose)
		{
			glUniformMatrix3fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Mat3x4Vector &value, const GLboolean transpose)
		{
			glUniformMatrix3x4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Mat4x2Vector &value, const GLboolean transpose)
		{
			glUniformMatrix4x2fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Mat4x3Vector &value, const GLboolean transpose)
		{
			glUniformMatrix4x3fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const Uint32 size,
			const Mat4x4Vector &value, const GLboolean transpose)
		{
			glUniformMatrix4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		void set_uniform(const Uint32 index, const glm::mat2x2 &value,
			const GLboolean transpose)
		{
			glUniformMatrix2fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::mat2x3 &value,
			const GLboolean transpose)
		{
			glUniformMatrix2x3fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::mat2x4 &value,
			const GLboolean transpose)
		{
			glUniformMatrix2x4fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::mat3x2 &value,
			const GLboolean transpose)
		{
			glUniformMatrix3x2fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::mat3x3 &value,
			const GLboolean transpose)
		{
			glUniformMatrix3fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::mat3x4 &value,
			const GLboolean transpose)
		{
			glUniformMatrix3x4fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::mat4x2 &value,
			const GLboolean transpose)
		{
			glUniformMatrix4x2fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::mat4x3 &value,
			const GLboolean transpose)
		{
			glUniformMatrix4x3fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		void set_uniform(const Uint32 index, const glm::mat4x4 &value,
			const GLboolean transpose)
		{
			glUniformMatrix4fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		#define CHECK_TYPE(parameter)	\
			do	\
			{	\
				if (m_parameter != parameter)	\
				{	\
					BoostFormat format_string(UTF8(	\
						"Uniform %1% has type %2%, but"	\
						" given type is %3%"));	\
					\
					format_string % m_name % m_parameter %	\
						parameter;	\
					\
					EL_THROW_EXCEPTION(VariantException()	\
						<< errinfo_message(	\
							format_string.str()));	\
				}	\
			}	\
			while (false)

		class GlUniformVariant: public boost::static_visitor<>
		{
			private:
				const String m_name;
				const GLint m_index;
				const Uint32 m_size;
				const ParameterType m_parameter;
				const GLboolean m_transpose;
				StringUint16Map &m_samplers;
				bool &m_sampler_changed;

			public:
				GlUniformVariant(const String &name,
					const GLint index,
					const Uint32 size,
					const ParameterType parameter,
					const GLboolean transpose,
					StringUint16Map &samplers,
					bool &sampler_changed): m_name(name),
					m_index(index), m_size(size),
					m_parameter(parameter),
					m_transpose(transpose),
					m_samplers(samplers),
					m_sampler_changed(sampler_changed)
				{
					m_sampler_changed = false;
				}

				~GlUniformVariant() throw()
				{
				}

				void operator()(const bool value) const
				{
					CHECK_TYPE(pt_bool);

					set_uniform(m_index, value);
				}

				void operator()(const float value) const
				{
					CHECK_TYPE(pt_float);

					set_uniform(m_index, value);
				}

				void operator()(const Sint64 value) const
				{
					StringUint16Map::iterator found;

					if (m_parameter == pt_int)
					{
						set_uniform(m_index,
							static_cast<Sint32>(
								value));
						return;
					}

					if (m_parameter == pt_unsigned_int)
					{
						set_uniform(m_index,
							static_cast<Uint32>(
								value));
						return;
					}

					if (ParameterUtil::get_sampler(
						m_parameter))
					{
						set_uniform(m_index,
							static_cast<Sint32>(
								value));

						found = m_samplers.find(m_name);

						if (found != m_samplers.end())
						{
							m_sampler_changed =
								found->second
								!= value;
							found->second = value;
						}
						else
						{
							m_sampler_changed =
								true;
							m_samplers[m_name] =
								value;
						}

						return;
					}

					BoostFormat format_string(UTF8(
						"Uniform %1% has type %2%, but"
						" given type is int"));

					format_string % m_name % m_parameter;

					EL_THROW_EXCEPTION(VariantException()
						<< errinfo_message(
							format_string.str()));
				}

				void operator()(const glm::vec2 &value) const
				{
					CHECK_TYPE(pt_vec2);

					set_uniform(m_index, value);
				}

				void operator()(const glm::vec3 &value) const
				{
					CHECK_TYPE(pt_vec3);

					set_uniform(m_index, value);
				}

				void operator()(const glm::vec4 &value) const
				{
					CHECK_TYPE(pt_vec4);

					set_uniform(m_index, value);
				}

				void operator()(const glm::ivec2 &value) const
				{
					CHECK_TYPE(pt_ivec2);

					set_uniform(m_index, value);
				}

				void operator()(const glm::ivec3 &value) const
				{
					CHECK_TYPE(pt_ivec3);

					set_uniform(m_index, value);
				}

				void operator()(const glm::ivec4 &value) const
				{
					CHECK_TYPE(pt_ivec4);

					set_uniform(m_index, value);
				}

				void operator()(const glm::uvec2 &value) const
				{
					CHECK_TYPE(pt_uvec2);

					set_uniform(m_index, value);
				}

				void operator()(const glm::uvec3 &value) const
				{
					CHECK_TYPE(pt_uvec3);

					set_uniform(m_index, value);
				}

				void operator()(const glm::uvec4 &value) const
				{
					CHECK_TYPE(pt_uvec4);

					set_uniform(m_index, value);
				}

				void operator()(const glm::bvec2 &value) const
				{
					CHECK_TYPE(pt_bvec2);

					set_uniform(m_index, value);
				}

				void operator()(const glm::bvec3 &value) const
				{
					CHECK_TYPE(pt_bvec3);

					set_uniform(m_index, value);
				}

				void operator()(const glm::bvec4 &value) const
				{
					CHECK_TYPE(pt_bvec4);

					set_uniform(m_index, value);
				}

				void operator()(const glm::mat2x2 &value) const
				{
					CHECK_TYPE(pt_mat2x2);

					set_uniform(m_index, value,
						m_transpose);
				}

				void operator()(const glm::mat2x3 &value) const
				{
					CHECK_TYPE(pt_mat2x3);

					set_uniform(m_index, value,
						m_transpose);
				}

				void operator()(const glm::mat2x4 &value) const
				{
					CHECK_TYPE(pt_mat2x4);

					set_uniform(m_index, value,
						m_transpose);
				}

				void operator()(const glm::mat3x2 &value) const
				{
					CHECK_TYPE(pt_mat3x2);

					set_uniform(m_index, value,
						m_transpose);
				}

				void operator()(const glm::mat3x3 &value) const
				{
					CHECK_TYPE(pt_mat3x3);

					set_uniform(m_index, value,
						m_transpose);
				}

				void operator()(const glm::mat3x4 &value) const
				{
					CHECK_TYPE(pt_mat3x4);

					set_uniform(m_index, value,
						m_transpose);
				}

				void operator()(const glm::mat4x2 &value) const
				{
					CHECK_TYPE(pt_mat4x2);

					set_uniform(m_index, value,
						m_transpose);
				}

				void operator()(const glm::mat4x3 &value) const
				{
					CHECK_TYPE(pt_mat4x3);

					set_uniform(m_index, value,
						m_transpose);
				}

				void operator()(const glm::mat4x4 &value) const
				{
					CHECK_TYPE(pt_mat4x4);

					set_uniform(m_index, value,
						m_transpose);
				}

				void operator()(const Ivec4Vector &value) const
				{
					CHECK_TYPE(pt_ivec4);

					set_uniform(m_index, m_size, value);
				}

				void operator()(const Uvec4Vector &value) const
				{
					CHECK_TYPE(pt_uvec4);

					set_uniform(m_index, m_size, value);
				}

				void operator()(const Vec4Vector &value) const
				{
					CHECK_TYPE(pt_vec4);

					set_uniform(m_index, m_size, value);
				}

				void operator()(const Mat2x4Vector &value) const
				{
					CHECK_TYPE(pt_mat2x4);

					set_uniform(m_index, m_size, value,
						m_transpose);
				}

				void operator()(const Mat3x4Vector &value) const
				{
					CHECK_TYPE(pt_mat3x4);

					set_uniform(m_index, m_size, value,
						m_transpose);
				}

				void operator()(const Mat4x4Vector &value) const
				{
					CHECK_TYPE(pt_mat4x4);

					set_uniform(m_index, m_size, value,
						m_transpose);
				}

		};

	}

	class GlslProgram::Uniform
	{
		public:
			ParameterType m_parameter;
			Uint32 m_index;
			Uint32 m_size;
	};

	GlslProgram::GlslProgram(const StringType &vertex_shader,
		const StringType &fragment_shader,
		const StringVariantMap &values, const String &name):
		m_name(name), m_last_used(0), m_program(0)
	{
		build(vertex_shader, fragment_shader, values);
	}

	GlslProgram::~GlslProgram() throw()
	{
		if (m_program != 0)
		{
			glDeleteProgram(m_program);
		}

		m_program = 0;
	}

	void GlslProgram::set_variant_parameter(const String &name,
		const Variant &value, const bool transpose)
	{
		StringUniformMap::const_iterator found;
		bool sampler_changed;

		assert(get_active());

		found = m_uniforms.find(name);

		if (found != m_uniforms.end())
		{
			boost::apply_visitor(GlUniformVariant(found->first,
				found->second.m_index, found->second.m_size,
				found->second.m_parameter, transpose,
				m_samplers, sampler_changed), value);

			if (sampler_changed)
			{
				update_used_texture_units();
			}
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const bool value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_bool);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::bvec2 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_bvec2);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::bvec3 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_bvec3);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::bvec4 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_bvec4);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Sint32 value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_int);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::ivec2 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_ivec2);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::ivec3 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_ivec3);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::ivec4 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_ivec4);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Uint32 value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_unsigned_int);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::uvec2 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_uvec2);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::uvec3 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_uvec3);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::uvec4 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_uvec4);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Sint32Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_int);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Ivec2Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_ivec2);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Ivec3Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_ivec3);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Ivec4Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_ivec4);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Uint32Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_unsigned_int);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Uvec2Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_uvec2);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Uvec3Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_uvec3);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Uvec4Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_uvec4);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const float value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_float);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const FloatVector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_float);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Vec2Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_vec2);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Vec3Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_vec3);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Vec4Vector &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_vec4);
			set_uniform(found->second.m_index,
				found->second.m_size, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::vec2 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_vec2);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::vec3 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_vec3);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::vec4 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_vec4);
			set_uniform(found->second.m_index, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat2x2Vector &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x2);
			set_uniform(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat2x3Vector &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x3);
			set_uniform(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat2x4Vector &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x4);
			set_uniform(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat3x2Vector &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat3x2);
			set_uniform(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat3x3Vector &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat3x3);
			set_uniform(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat3x4Vector &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat3x4);
			set_uniform(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat4x2Vector &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat4x2);
			set_uniform(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat4x3Vector &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat4x3);
			set_uniform(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat4x4Vector &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat4x4);
			set_uniform(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::mat2x2 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x2);
			set_uniform(found->second.m_index, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::mat2x3 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x3);
			set_uniform(found->second.m_index, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::mat2x4 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x4);
			set_uniform(found->second.m_index, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::mat3x2 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat3x2);
			set_uniform(found->second.m_index, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::mat3x3 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat3x3);
			set_uniform(found->second.m_index, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::mat3x4 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat3x4);
			set_uniform(found->second.m_index, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::mat4x2 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat4x2);
			set_uniform(found->second.m_index, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::mat4x3 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat4x3);
			set_uniform(found->second.m_index, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const glm::mat4x4 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat4x4);
			set_uniform(found->second.m_index, value, transpose);
		}
	}

	StringType GlslProgram::get_program_log()
	{
		GLcharSharedArray buffer;
		GLint blen, slen;

		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &blen);

		if (blen > 0)
		{
			buffer.reset(new GLchar[blen]);

			slen = blen;

			glGetProgramInfoLog(m_program, blen, &slen, buffer.get());

			slen = std::min((GLint)strlen(buffer.get()), slen);

			return std::string(buffer.get(), slen);
		}
		else
		{
			return UTF8("");
		}
	}

	bool GlslProgram::program_link_status()
	{
		GLint ret;

		glGetProgramiv(m_program, GL_LINK_STATUS, &ret);

		return ret == GL_TRUE;
	}

	bool GlslProgram::program_validate_status()
	{
		GLint ret;

		glGetProgramiv(m_program, GL_VALIDATE_STATUS, &ret);

		return ret == GL_TRUE;
	}

	void GlslProgram::bind()
	{
		glUseProgram(m_program);

		assert(get_active());
	}

	GLuint GlslProgram::get_current_program()
	{
		GLint program;

		glGetIntegerv(GL_CURRENT_PROGRAM, &program);

		return static_cast<GLuint>(program);
	}

	void GlslProgram::bind_attribute_location(const VertexSemanticType type)
	{
		glBindAttribLocation(m_program, type,
			VertexElement::get_str(type).get().c_str());
	}

	void GlslProgram::bind_attribute_locations()
	{
		bind_attribute_location(vst_position);
		bind_attribute_location(vst_normal);
		bind_attribute_location(vst_tangent);
		bind_attribute_location(vst_color);
		bind_attribute_location(vst_bone_weight);
		bind_attribute_location(vst_bone_index);
		bind_attribute_location(vst_extra_bone_weight);
		bind_attribute_location(vst_extra_bone_index);
		bind_attribute_location(vst_texture_coordinate_0);
		bind_attribute_location(vst_texture_coordinate_1);
		bind_attribute_location(vst_layer_index);
		bind_attribute_location(vst_morph_position);
		bind_attribute_location(vst_morph_normal);
		bind_attribute_location(vst_morph_tangent);
		bind_attribute_location(vst_morph_texture_coordinate_0);
		bind_attribute_location(vst_morph_texture_coordinate_1);
	}

	void GlslProgram::log_uniforms()
	{
		Uniform uniform;
		String name;
		GLcharSharedArray buffer;
		GLint count, max_buffer_size;
		GLint index, length;
		GLint i, array_size;
		GLenum type;
		ParameterType parameter;
		AutoParameterType auto_parameter;

		glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
			&max_buffer_size);

		buffer.reset(new GLchar[max_buffer_size + 1]);

		glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &count);

		for (i = 0; i < count; ++i)
		{
			type = 0;
			length = 0;
			array_size = 0;

			glGetActiveUniform(m_program, i, max_buffer_size,
				&length, &array_size, &type, buffer.get());

			assert(array_size > 0);

			name = String(get_base_name(std::string(buffer.get(),
				length)));
			index = glGetUniformLocation(m_program, buffer.get());
			parameter = ParameterUtil::get_parameter(type);

			if (index >= 0)
			{
				uniform.m_index = index;
				uniform.m_size = array_size;
				uniform.m_parameter = parameter;

				LOG_DEBUG(UTF8("\tUniform %1% of type %2% has "
					"size %3% and index %4%."), name %
					parameter % array_size % index);

				m_uniforms[name] = uniform;

				if (AutoParameterUtil::get_auto_parameter(
					name, auto_parameter))
				{
					assert(AutoParameterUtil::get_type(
						auto_parameter) ==
							parameter);
					m_auto_parameters[auto_parameter]
						= uniform;
				}

				if (ParameterUtil::get_sampler(parameter))
				{
					m_samplers[name] = 0;
				}
			}
		}
	}

	void GlslProgram::log_attribute_locations()
	{
		String name;
		ParameterType parameter;
		GLcharSharedArray buffer;
		GLint count, max_buffer_size;
		GLint index, length;
		GLenum type;
		GLint i, array_size;

		glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
			&max_buffer_size);

		buffer.reset(new GLchar[max_buffer_size + 1]);

		glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &count);

		for (i = 0; i < count; ++i)
		{
			type = 0;
			length = 0;
			array_size = 0;

			glGetActiveAttrib(m_program, i, max_buffer_size,
				&length, &array_size, &type, buffer.get());

			assert(array_size > 0);

			name = get_base_name(std::string(buffer.get(), length));
			index = glGetAttribLocation(m_program, buffer.get());
			parameter = ParameterUtil::get_parameter(type);

			LOG_DEBUG(UTF8("\tAttribute %1% of type %2% has size "
				"%3% and index %4%."), name % parameter %
				array_size % index);
		}
	}

	void GlslProgram::check()
	{
		glValidateProgram(m_program);

		if (!program_validate_status())
		{
			EL_THROW_EXCEPTION(OpenGlException()
				<< errinfo_message(get_program_log()));
		}

		LOG_INFO(UTF8("Program validate successful: %1%"),
			get_program_log());
	}

	void GlslProgram::unbind()
	{
		glUseProgram(0);
	}

	bool GlslProgram::get_active() const
	{
		return m_program == get_current_program();
	}

	void GlslProgram::do_build(const StringType &vertex_shader,
		const StringType &fragment_shader)
	{
		GlslShaderObject vertex(GL_VERTEX_SHADER);
		GlslShaderObject fragment(GL_FRAGMENT_SHADER);

		m_program = glCreateProgram();

		vertex.load(vertex_shader);
		vertex.compile();
		glAttachShader(get_program(), vertex.get_shader());

		fragment.load(fragment_shader);
		fragment.compile();
		glAttachShader(get_program(), fragment.get_shader());

		bind_attribute_locations();

		glLinkProgram(get_program());

		if (!program_link_status())
		{
			EL_THROW_EXCEPTION(OpenGlException()
				<< errinfo_message(get_program_log()));
		}

		LOG_DEBUG(UTF8("Program linked successful: %1%"),
			get_program_log());

		log_attribute_locations();
		log_uniforms();
	}

	void GlslProgram::build(const StringType &vertex_shader,
		const StringType &fragment_shader,
		const StringVariantMap &values)
	{
		StringVariantMap::const_iterator it, end;

		LOG_DEBUG(UTF8("Building Shader %1%"), get_name());

		try
		{
			do_build(vertex_shader, fragment_shader);
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_parameter_name(get_name().get());
			exception << errinfo_vertex_shader_source(
				vertex_shader);
			exception << errinfo_fragment_shader_source(
				fragment_shader);

			throw;
		}

		bind();

		end = values.end();

		for (it = values.begin(); it != end; ++it)
		{
			set_variant_parameter(it->first, it->second);
		}

		unbind();

		update_used_texture_units();

		m_last_used = 0;
	}

	void GlslProgram::update_used_texture_units()
	{
		StringUint16Map::const_iterator it, end;

		m_used_texture_units.reset();

		end = m_samplers.end();

		for (it = m_samplers.begin(); it != end; ++it)
		{
			if (m_used_texture_units.size() > it->second)
			{
				m_used_texture_units[it->second] = true;
			}
		}
	}

}