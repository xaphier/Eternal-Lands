/****************************************************************************
 *	    glslprogram.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "glslprogram.hpp"
#include "glslprogramdescription.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "shader/parameterutil.hpp"
#include "shader/samplerparameterutil.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "shader/uniformbufferdescription.hpp"
#include "shader/uniformdescription.hpp"
#include "shader/uniformbufferutil.hpp"
#include "uniformbufferdescriptioncache.hpp"

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
				~GlslShaderObject() noexcept;
				void load(const StringType &source);
				StringType get_shader_log() const;
				bool get_shader_compile_status() const;
				void compile();

				inline GLuint get_shader() const noexcept
				{
					return m_shader;
				}

		};

		GlslShaderObject::GlslShaderObject(const GLenum shader_type):
			m_shader(0)
		{
			assert((shader_type == GL_VERTEX_SHADER) ||
				(shader_type == GL_TESS_CONTROL_SHADER) ||
				(shader_type == GL_TESS_EVALUATION_SHADER) ||
				(shader_type == GL_GEOMETRY_SHADER) ||
				(shader_type == GL_FRAGMENT_SHADER));

			m_shader = glCreateShader(shader_type);

			assert(m_shader != 0);
		}

		GlslShaderObject::~GlslShaderObject() noexcept
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

			LOG_DEBUG(lt_glsl_program, UTF8("GlslShader compiled "
				"successful: %1%"), get_shader_log());
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

		inline void set_uniform(const Uint32 index, const bool value)
			noexcept
		{
			glUniform1i(index, value);
		}

		inline void set_uniform(const Uint32 index,
			const glm::bvec2 &value) noexcept
		{
			glUniform2i(index, value[0], value[1]);
		}

		inline void set_uniform(const Uint32 index,
			const glm::bvec3 &value) noexcept
		{
			glUniform3i(index, value[0], value[1], value[2]);
		}

		inline void set_uniform(const Uint32 index,
			const glm::bvec4 &value) noexcept
		{
			glUniform4i(index, value[0], value[1], value[2],
				value[3]);
		}

		inline void set_uniform(const Uint32 index,
			const Sint32 value) noexcept
		{
			glUniform1i(index, value);
		}

		inline void set_uniform(const Uint32 index,
			const glm::ivec2 &value) noexcept
		{
			glUniform2iv(index, 1, glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::ivec3 &value) noexcept
		{
			glUniform3iv(index, 1, glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::ivec4 &value) noexcept
		{
			glUniform4iv(index, 1, glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Sint32Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform1iv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				&value[offset]);
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Ivec2Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform2iv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Ivec3Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform3iv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Ivec4Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform4iv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index,
			const Uint32 value) noexcept
		{
			glUniform1ui(index, value);
		}

		inline void set_uniform(const Uint32 index,
			const glm::uvec2 &value) noexcept
		{
			glUniform2uiv(index, 1, glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::uvec3 &value) noexcept
		{
			glUniform3uiv(index, 1, glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::uvec4 &value) noexcept
		{
			glUniform4uiv(index, 1, glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Uint32Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform1uiv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				&value[offset]);
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Uvec2Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform2uiv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Uvec3Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform3uiv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Uvec4Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform4uiv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const float value)
			noexcept
		{
			glUniform1f(index, value);
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const FloatVector &value) noexcept
		{
			assert(offset < value.size());
			glUniform1fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				&value[offset]);
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Vec2Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform2fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Vec3Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform3fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Vec4Vector &value) noexcept
		{
			assert(offset < value.size());
			glUniform4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index,
			const glm::vec2 &value) noexcept
		{
			glUniform2fv(index, 1, glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::vec3 &value) noexcept
		{
			glUniform3fv(index, 1, glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::vec4 &value) noexcept
		{
			glUniform4fv(index, 1, glm::value_ptr(value));
		}

		template<Uint32 N>
		inline void set_uniform(const Uint32 index, const Uint32 size,
			const boost::array<glm::vec4, N> &value) noexcept
		{
			glUniform4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				glm::value_ptr(value[0]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Mat2x2Vector &value,
			const GLboolean transpose) noexcept
		{
			assert(offset < value.size());
			glUniformMatrix2fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Mat2x3Vector &value,
			const GLboolean transpose) noexcept
		{
			assert(offset < value.size());
			glUniformMatrix2x3fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Mat2x4Vector &value,
			const GLboolean transpose) noexcept
		{
			assert(offset < value.size());
			glUniformMatrix2x4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Mat3x2Vector &value,
			const GLboolean transpose) noexcept
		{
			assert(offset < value.size());
			glUniformMatrix3x2fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Mat3x3Vector &value,
			const GLboolean transpose) noexcept
		{
			assert(offset < value.size());
			glUniformMatrix3fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Mat3x4Vector &value,
			const GLboolean transpose) noexcept
		{
			assert(offset < value.size());
			glUniformMatrix3x4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Mat4x2Vector &value,
			const GLboolean transpose) noexcept
		{
			assert(offset < value.size());
			glUniformMatrix4x2fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Mat4x3Vector &value,
			const GLboolean transpose) noexcept
		{
			assert(offset < value.size());
			glUniformMatrix4x3fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index, const Uint32 size,
			const Uint32 offset, const Mat4x4Vector &value,
			const GLboolean transpose) noexcept
		{
			assert(offset < value.size());
			glUniformMatrix4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[offset]));
		}

		inline void set_uniform(const Uint32 index,
			const glm::mat2x2 &value, const GLboolean transpose)
			noexcept
		{
			glUniformMatrix2fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::mat2x3 &value, const GLboolean transpose)
			noexcept
		{
			glUniformMatrix2x3fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		template<Uint32 N>
		inline void set_uniform(const Uint32 index, const Uint32 size,
			const boost::array<glm::mat2x3, N> &value,
			const GLboolean transpose) noexcept
		{
			glUniformMatrix2x3fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		inline void set_uniform(const Uint32 index,
			const glm::mat2x4 &value, const GLboolean transpose)
			noexcept
		{
			glUniformMatrix2x4fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		template<Uint32 N>
		inline void set_uniform(const Uint32 index, const Uint32 size,
			const boost::array<glm::mat2x4, N> &value,
			const GLboolean transpose) noexcept
		{
			glUniformMatrix2x4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		inline void set_uniform(const Uint32 index,
			const glm::mat3x2 &value, const GLboolean transpose)
			noexcept
		{
			glUniformMatrix3x2fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::mat3x3 &value, const GLboolean transpose)
			noexcept
		{
			glUniformMatrix3fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::mat3x4 &value, const GLboolean transpose)
			noexcept
		{
			glUniformMatrix3x4fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		template<Uint32 N>
		inline void set_uniform(const Uint32 index, const Uint32 size,
			const boost::array<glm::mat3x4, N> &value,
			const GLboolean transpose) noexcept
		{
			glUniformMatrix3x4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
		}

		inline void set_uniform(const Uint32 index,
			const glm::mat4x2 &value, const GLboolean transpose)
			noexcept
		{
			glUniformMatrix4x2fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::mat4x3 &value, const GLboolean transpose)
			noexcept
		{
			glUniformMatrix4x3fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		inline void set_uniform(const Uint32 index,
			const glm::mat4x4 &value, const GLboolean transpose)
			noexcept
		{
			glUniformMatrix4fv(index, 1, transpose,
				glm::value_ptr(value));
		}

		template<Uint32 N>
		inline void set_uniform(const Uint32 index, const Uint32 size,
			const boost::array<glm::mat4x4, N> &value,
			const GLboolean transpose) noexcept
		{
			glUniformMatrix4fv(index, std::min(size,
					static_cast<Uint32>(value.size())),
				transpose, glm::value_ptr(value[0]));
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
				const Uint32 m_offset;
				const ParameterType m_parameter;
				const GLboolean m_transpose;
				StringUint16Map &m_samplers;
				bool &m_sampler_changed;

			public:
				inline GlUniformVariant(const String &name,
					const GLint index, const Uint32 size,
					const Uint32 offset, 
					const ParameterType parameter,
					const GLboolean transpose,
					StringUint16Map &samplers,
					bool &sampler_changed): m_name(name),
					m_index(index), m_size(size),
					m_offset(offset),
					m_parameter(parameter),
					m_transpose(transpose),
					m_samplers(samplers),
					m_sampler_changed(sampler_changed)
				{
					m_sampler_changed = false;
				}

				inline ~GlUniformVariant() noexcept
				{
				}

				inline void operator()(const bool value) const
					noexcept
				{
					CHECK_TYPE(pt_bool);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const float value) const
					noexcept
				{
					CHECK_TYPE(pt_float);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const Sint64 value) const
				{
					StringUint16Map::iterator found;

					assert(m_size == 1);

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

				inline void operator()(const glm::vec2 &value)
					const noexcept
				{
					CHECK_TYPE(pt_vec2);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::vec3 &value)
					const noexcept
				{
					CHECK_TYPE(pt_vec3);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::vec4 &value)
					const noexcept
				{
					CHECK_TYPE(pt_vec4);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::ivec2 &value)
					const noexcept
				{
					CHECK_TYPE(pt_ivec2);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::ivec3 &value)
					const noexcept
				{
					CHECK_TYPE(pt_ivec3);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::ivec4 &value)
					const noexcept
				{
					CHECK_TYPE(pt_ivec4);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::uvec2 &value)
					const noexcept
				{
					CHECK_TYPE(pt_uvec2);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::uvec3 &value)
					const noexcept
				{
					CHECK_TYPE(pt_uvec3);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::uvec4 &value)
					const noexcept
				{
					CHECK_TYPE(pt_uvec4);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::bvec2 &value)
					const noexcept
				{
					CHECK_TYPE(pt_bvec2);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::bvec3 &value)
					const noexcept
				{
					CHECK_TYPE(pt_bvec3);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::bvec4 &value)
					const noexcept
				{
					CHECK_TYPE(pt_bvec4);

					assert(m_size == 1);

					set_uniform(m_index, value);
				}

				inline void operator()(const glm::mat2x2 &value)
					const noexcept
				{
					CHECK_TYPE(pt_mat2x2);

					assert(m_size == 1);

					set_uniform(m_index, value,
						m_transpose);
				}

				inline void operator()(const glm::mat2x3 &value)
					const noexcept
				{
					CHECK_TYPE(pt_mat2x3);

					assert(m_size == 1);

					set_uniform(m_index, value,
						m_transpose);
				}

				inline void operator()(const glm::mat2x4 &value)
					const noexcept
				{
					CHECK_TYPE(pt_mat2x4);

					assert(m_size == 1);

					set_uniform(m_index, value,
						m_transpose);
				}

				inline void operator()(const glm::mat3x2 &value)
					const noexcept
				{
					CHECK_TYPE(pt_mat3x2);

					assert(m_size == 1);

					set_uniform(m_index, value,
						m_transpose);
				}

				inline void operator()(const glm::mat3x3 &value)
					const noexcept
				{
					CHECK_TYPE(pt_mat3x3);

					assert(m_size == 1);

					set_uniform(m_index, value,
						m_transpose);
				}

				inline void operator()(const glm::mat3x4 &value)
					const noexcept
				{
					CHECK_TYPE(pt_mat3x4);

					assert(m_size == 1);

					set_uniform(m_index, value,
						m_transpose);
				}

				inline void operator()(const glm::mat4x2 &value)
					const noexcept
				{
					CHECK_TYPE(pt_mat4x2);

					assert(m_size == 1);

					set_uniform(m_index, value,
						m_transpose);
				}

				inline void operator()(const glm::mat4x3 &value)
					const noexcept
				{
					CHECK_TYPE(pt_mat4x3);

					assert(m_size == 1);

					set_uniform(m_index, value,
						m_transpose);
				}

				inline void operator()(const glm::mat4x4 &value)
					const noexcept
				{
					CHECK_TYPE(pt_mat4x4);

					assert(m_size == 1);

					set_uniform(m_index, value,
						m_transpose);
				}

				inline void operator()(const Ivec4Vector &value)
					const noexcept
				{
					CHECK_TYPE(pt_ivec4);

					assert(m_size >= 1);

					set_uniform(m_index, m_size, m_offset,
						value);
				}

				inline void operator()(const Uvec4Vector &value)
					const noexcept
				{
					CHECK_TYPE(pt_uvec4);

					assert(m_size >= 1);

					set_uniform(m_index, m_size, m_offset,
						value);
				}

				inline void operator()(const Vec4Vector &value)
					const noexcept
				{
					CHECK_TYPE(pt_vec4);

					assert(m_size >= 1);

					set_uniform(m_index, m_size, m_offset,
						value);
				}

				inline void operator()(
					const Mat2x4Vector &value) const
					noexcept
				{
					CHECK_TYPE(pt_mat2x4);

					assert(m_size >= 1);

					set_uniform(m_index, m_size, m_offset,
						value, m_transpose);
				}

				inline void operator()(
					const Mat3x4Vector &value) const
					noexcept
				{
					CHECK_TYPE(pt_mat3x4);

					assert(m_size >= 1);

					set_uniform(m_index, m_size, m_offset,
						value, m_transpose);
				}

				inline void operator()(
					const Mat4x4Vector &value) const
					noexcept
				{
					CHECK_TYPE(pt_mat4x4);

					assert(m_size >= 1);

					set_uniform(m_index, m_size, m_offset,
						value, m_transpose);
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

	GlslProgram::GlslProgram(const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const GlslProgramDescription &description,
		const boost::uuids::uuid &uuid): m_uuid(uuid), m_last_used(0),
		m_program(0)
	{
		build(uniform_buffer_description_cache, description);
	}

	GlslProgram::GlslProgram(const FileSystemSharedPtr &file_system,
		const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const String &file_name, const boost::uuids::uuid &uuid):
		m_uuid(uuid), m_last_used(0), m_program(0)
	{
		load_xml(uniform_buffer_description_cache, file_system,
			file_name);
	}

	GlslProgram::GlslProgram(const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const String &file_name, const boost::uuids::uuid &uuid):
		m_uuid(uuid), m_last_used(0), m_program(0)
	{
		load_xml(uniform_buffer_description_cache, file_name);
	}

	GlslProgram::~GlslProgram() noexcept
	{
		if (m_program != 0)
		{
			glDeleteProgram(m_program);
		}

		m_program = 0;
	}

	void GlslProgram::set_variant_parameter(const String &name,
		const Variant &value, const Uint32 offset, const bool transpose)
	{
		StringUniformMap::const_iterator found;
		bool sampler_changed;

		assert(get_active());

		found = m_uniforms.find(name);

		if (found != m_uniforms.end())
		{
			boost::apply_visitor(GlUniformVariant(found->first,
				found->second.m_index, found->second.m_size,
				offset, found->second.m_parameter, transpose,
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
		const Sint32Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_int);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Ivec2Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_ivec2);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Ivec3Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_ivec3);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Ivec4Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_ivec4);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Uint32Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_unsigned_int);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Uvec2Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_uvec2);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Uvec3Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_uvec3);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Uvec4Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_uvec4);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
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
		const FloatVector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_float);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Vec2Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_vec2);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Vec3Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_vec3);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Vec4Vector &value, const Uint32 offset)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_vec4);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value);
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
		const Mat2x2Vector &value, const Uint32 offset,
		const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x2);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat2x3Vector &value, const Uint32 offset,
		const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x3);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat2x4Vector &value, const Uint32 offset,
		const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x4);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat3x2Vector &value, const Uint32 offset,
		const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat3x2);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat3x3Vector &value, const Uint32 offset,
		const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat3x3);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat3x4Vector &value, const Uint32 offset,
		const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat3x4);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat4x2Vector &value, const Uint32 offset,
		const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat4x2);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat4x3Vector &value, const Uint32 offset,
		const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat4x3);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat4x4Vector &value, const Uint32 offset,
		const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat4x4);
			set_uniform(found->second.m_index,
				found->second.m_size, offset, value, transpose);
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

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat2x3Array2 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x3);
			set_uniform<2>(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Mat2x4Array4 &value, const bool transpose)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_mat2x4);
			set_uniform<4>(found->second.m_index,
				found->second.m_size, value, transpose);
		}
	}

	void GlslProgram::set_parameter(const AutoParameterType parameter,
		const Vec4Array4 &value)
	{
		AutoParameterTypeUniformMap::const_iterator found;

		assert(get_active());

		found = m_auto_parameters.find(parameter);

		if (found != m_auto_parameters.end())
		{
			assert(found->second.m_parameter == pt_vec4);
			set_uniform<4>(found->second.m_index,
				found->second.m_size, value);
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
		bind_attribute_location(vst_texture_coordinate);
		bind_attribute_location(vst_morph_position);
		bind_attribute_location(vst_morph_normal);
		bind_attribute_location(vst_morph_tangent);
		bind_attribute_location(vst_morph_texture_coordinate);
		bind_attribute_location(vst_world_matrix_0);
		bind_attribute_location(vst_world_matrix_1);
		bind_attribute_location(vst_world_matrix_2);
	}

	void GlslProgram::check_uniform_buffer(
		const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const Uint32 index)
	{
		UniformBufferDescriptionSharedPtr uniform_buffer_description;
		boost::scoped_array<GLint> uniform_block_active_uniform_indices;
		StringStream str;
		String name, tmp_str0, tmp_str1;
		GLcharSharedArray buffer;
		UniformBufferType uniform_buffer;
		GLint i, max_buffer_size, length;
		GLint uniform_type, uniform_size, uniform_name_length;
		GLint uniform_block_index, uniform_offset, uniform_array_stride;
		GLint uniform_matrix_stride, uniform_is_row_major;
		GLuint uniform_index;
		GLint uniform_block_binding, uniform_block_data_size;
		GLint uniform_block_name_length, uniform_block_active_uniforms;
		ParameterType type;
		AutoParameterType auto_parameter;
		bool init;

		glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
			&max_buffer_size);

		buffer.reset(new GLchar[max_buffer_size + 1]);

		glGetActiveUniformBlockName(m_program, index, max_buffer_size,
			&length, buffer.get());
		glGetActiveUniformBlockiv(m_program, index,
			GL_UNIFORM_BLOCK_BINDING, &uniform_block_binding);
		glGetActiveUniformBlockiv(m_program, index,
			GL_UNIFORM_BLOCK_DATA_SIZE, &uniform_block_data_size);
		glGetActiveUniformBlockiv(m_program, index,
			GL_UNIFORM_BLOCK_NAME_LENGTH,
			&uniform_block_name_length);
		glGetActiveUniformBlockiv(m_program, index,
			GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
			&uniform_block_active_uniforms);

		name = String(std::string(buffer.get(), length));

		if (uniform_block_active_uniforms < 1)
		{
			LOG_DEBUG(lt_glsl_program, UTF8("Uniform block '%1%' "
				"has no active uniforms"), name);

			return;
		}

		if (!UniformBufferUtil::get_uniform_buffer_from_identifier(name,
			uniform_buffer))
		{
			LOG_ERROR(lt_glsl_program, UTF8("Uniform block '%1%' "
				"has unkown name"), name);

			return;
		}

		if (uniform_block_data_size < 1)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_parameter_name(
					UTF8("uniform_block_data_size"))
				<< errinfo_range_index(uniform_block_data_size)
				<< errinfo_range_min(1));
		}

		uniform_buffer_description = uniform_buffer_description_cache->
			get_uniform_buffer_description(uniform_buffer);

		init = uniform_buffer_description->get_size() == 0;

		if (init)
		{
			uniform_buffer_description->set_size(
				uniform_block_data_size);
		}

		if (uniform_buffer_description->get_size() !=
			static_cast<Uint32>(uniform_block_data_size))
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_value(uniform_block_data_size)
				<< errinfo_expected_value(
					uniform_buffer_description->
						get_size()));
		}

		uniform_block_active_uniform_indices.reset(
			new GLint[uniform_block_active_uniforms]);

		glGetActiveUniformBlockiv(m_program, index,
			GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
			uniform_block_active_uniform_indices.get());

		BoostFormat format(UTF8("\t%1% binding %2% data size "
			"%3% active uniforms %4%."));

		format % name % uniform_block_binding;
		format % uniform_block_data_size;
		format % uniform_block_active_uniforms;

		str << format.str() << std::endl;

		glUniformBlockBinding(m_program, index, uniform_buffer);

		for (i = 0; i < uniform_block_active_uniforms; ++i)
		{
			uniform_index = uniform_block_active_uniform_indices[i];

			uniform_type = 0;
			uniform_size = 0;
			uniform_name_length = 0;
			uniform_block_index = 0;
			uniform_offset = 0;
			uniform_array_stride = 0;
			uniform_matrix_stride = 0;
			uniform_is_row_major = 0;

			glGetActiveUniformsiv(m_program, 1, &uniform_index,
				GL_UNIFORM_TYPE, &uniform_type);
			glGetActiveUniformsiv(m_program, 1, &uniform_index,
				GL_UNIFORM_SIZE, &uniform_size);
			glGetActiveUniformsiv(m_program, 1, &uniform_index,
				GL_UNIFORM_NAME_LENGTH, &uniform_name_length);
			glGetActiveUniformsiv(m_program, 1, &uniform_index,
				GL_UNIFORM_BLOCK_INDEX,	&uniform_block_index);
			glGetActiveUniformsiv(m_program, 1, &uniform_index,
				GL_UNIFORM_OFFSET, &uniform_offset);
			glGetActiveUniformsiv(m_program, 1, &uniform_index,
				GL_UNIFORM_ARRAY_STRIDE, &uniform_array_stride);
			glGetActiveUniformsiv(m_program, 1, &uniform_index,
				GL_UNIFORM_MATRIX_STRIDE,
				&uniform_matrix_stride);
			glGetActiveUniformsiv(m_program, 1, &uniform_index,
				GL_UNIFORM_IS_ROW_MAJOR,
				&uniform_is_row_major);

			glGetActiveUniformName(m_program, uniform_index,
				max_buffer_size, &length, buffer.get());

			name = String(get_base_name(std::string(buffer.get(),
				length)));

			type = ParameterUtil::get_parameter(uniform_type);

			auto_parameter = AutoParameterUtil::get_auto_parameter(
				name);

			UniformDescription &uniform_description =
				uniform_buffer_description->
					get_uniform_description(auto_parameter);

			if (uniform_size < 1)
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("uniform_size"))
					<< errinfo_range_index(uniform_size)
					<< errinfo_range_min(1));
			}

			if (static_cast<Sint64>(uniform_block_index) !=
				static_cast<Sint64>(index))
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("uniform_block_index"))
					<< errinfo_value(uniform_block_index)
					<< errinfo_expected_value(index));
			}

			if (uniform_offset < 0)
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("uniform_offset"))
					<< errinfo_range_index(uniform_offset)
					<< errinfo_range_min(0));
			}

			if (uniform_array_stride < 0)
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("uniform_array_stride"))
					<< errinfo_range_index(
						uniform_array_stride)
					<< errinfo_range_min(0));
			}

			if (uniform_matrix_stride < 0)
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("uniform_matrix_stride"))
					<< errinfo_range_index(
						uniform_matrix_stride)
					<< errinfo_range_min(0));
			}

			if (init)
			{
				uniform_description.set_size(uniform_size);
				uniform_description.set_offset(uniform_offset);
				uniform_description.set_array_stride(
					uniform_array_stride);
				uniform_description.set_matrix_stride(
					uniform_matrix_stride);
				uniform_description.set_is_row_major(
					uniform_is_row_major == GL_TRUE);
			}

			if (uniform_description.get_type() != type)
			{
				tmp_str0 = ParameterUtil::get_str(type);
				tmp_str1 = ParameterUtil::get_str(
					uniform_description.get_type());

				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("type"))
					<< errinfo_string_value(tmp_str0)
					<< errinfo_expected_string_value(
						tmp_str1));
			}

			if (uniform_description.get_size() !=
				static_cast<Uint32>(uniform_size))
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("uniform_size"))
					<< errinfo_value(uniform_size)
					<< errinfo_expected_value(
						uniform_description.
							get_size()));
			}

			if (uniform_description.get_offset() !=
				static_cast<Uint32>(uniform_offset))
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("uniform_offset"))
					<< errinfo_value(uniform_offset)
					<< errinfo_expected_value(
						uniform_description.
							get_offset()));
			}

			if (uniform_description.get_array_stride() !=
				static_cast<Uint32>(uniform_array_stride))
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("uniform_array_stride"))
					<< errinfo_value(uniform_array_stride)
					<< errinfo_expected_value(
						uniform_description.
							get_array_stride()));
			}

			if (uniform_description.get_matrix_stride() !=
				static_cast<Uint32>(uniform_matrix_stride))
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("uniform_matrix_stride"))
					<< errinfo_value(uniform_matrix_stride)
					<< errinfo_expected_value(
						uniform_description.
							get_matrix_stride()));
			}

			if (uniform_description.get_is_row_major() !=
				(uniform_is_row_major == GL_TRUE))
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(
						UTF8("uniform_is_row_major"))
					<< errinfo_value(uniform_is_row_major
						== GL_TRUE)
					<< errinfo_expected_value(
						uniform_description.
							get_is_row_major()));
			}

			BoostFormat format(UTF8("\t\t%1% of type %2% has size"
				" %3% block index %4% offset %5% array stride"
				" %6% matrix stride %7% is row major %8%."));

			format % name % type % uniform_size;
			format % uniform_block_index % uniform_offset;
			format % uniform_array_stride;
			format % uniform_matrix_stride;
			format % uniform_is_row_major;

			str << format.str() << std::endl;
		}

		LOG_DEBUG(lt_glsl_program, UTF8("Uniform block:\n%1%"),
			str.str());
	}

	void GlslProgram::log_uniforms(
		const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache)
	{
		Uniform uniform;
		StringStream str;
		String name, tmp_str0, tmp_str1;
		GLcharSharedArray buffer;
		GLint count, max_buffer_size;
		GLint index, length;
		GLint i, array_size;
		GLenum type;
		ParameterType parameter, tmp;
		AutoParameterType auto_parameter;

		glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
			&max_buffer_size);

		buffer.reset(new GLchar[max_buffer_size + 1]);

		glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &count);

		for (i = 0; i < count; ++i)
		{
			BoostFormat format(UTF8("\t%1% of type %2% has size "
				"%3% and index %4%."));

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

			if (index < 0)
			{
				continue;
			}

			uniform.m_index = index;
			uniform.m_size = array_size;
			uniform.m_parameter = parameter;

			format % name % parameter % array_size % index;

			str << format.str() << std::endl;

			m_uniforms[name] = uniform;

			if (AutoParameterUtil::get_auto_parameter(name,
				auto_parameter))
			{
				tmp = AutoParameterUtil::get_type(
					auto_parameter);

				if (tmp != parameter)
				{
					tmp_str0 = ParameterUtil::get_str(
						parameter);
					tmp_str1 = ParameterUtil::get_str(
						tmp);

					EL_THROW_EXCEPTION(
						InvalidParameterException()
						<< errinfo_parameter_name(
							UTF8("parameter"))
						<< errinfo_string_value(
							tmp_str0)
						<< errinfo_expected_string_value(
							tmp_str1));
				}

				m_auto_parameters[auto_parameter] = uniform;
			}

			if (ParameterUtil::get_sampler(parameter))
			{
				m_samplers[name] = 0;
			}
		}

		LOG_DEBUG(lt_glsl_program, UTF8("Uniforms:\n%1%"), str.str());

		if (!GLEW_VERSION_3_1)
		{
			return;
		}

		glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_BLOCKS, &count);

		for (i = 0; i < count; ++i)
		{
			check_uniform_buffer(uniform_buffer_description_cache,
				i);
		}
	}

	void GlslProgram::log_attribute_locations()
	{
		StringStream str;
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

		m_used_attributes.reset();

		for (i = 0; i < count; ++i)
		{
			BoostFormat format(UTF8("\t%1% of type %2% has size "
				"%3% and index %4%."));

			type = 0;
			length = 0;
			array_size = 0;

			glGetActiveAttrib(m_program, i, max_buffer_size,
				&length, &array_size, &type, buffer.get());

			assert(array_size > 0);

			name = get_base_name(std::string(buffer.get(), length));
			index = glGetAttribLocation(m_program, buffer.get());
			parameter = ParameterUtil::get_parameter(type);

			m_used_attributes[index] = true;

			format % name % parameter % array_size % index;

			str << format.str() << std::endl;
		}

		LOG_DEBUG(lt_glsl_program, UTF8("Attributes:\n%1%"), str.str());
	}

	void GlslProgram::check()
	{
		glValidateProgram(m_program);

		if (!program_validate_status())
		{
			EL_THROW_EXCEPTION(OpenGlException()
				<< errinfo_message(get_program_log()));
		}

		LOG_DEBUG(lt_glsl_program, UTF8("Program validate successful:"
			" %1%"), get_program_log());
	}

	bool GlslProgram::validate()
	{
		glValidateProgram(m_program);

		return program_validate_status();
	}

	void GlslProgram::log_validate_status()
	{
		glValidateProgram(m_program);

		if (!program_validate_status())
		{
			LOG_ERROR(lt_glsl_program, UTF8("Program '%1%' "
				"validation: %2%"), get_uuid() %
				get_program_log());
			return;
		}

		LOG_INFO(lt_glsl_program, UTF8("Program '%1%' validate "
			"successful: %2%"), get_uuid() % get_program_log());
	}

	void GlslProgram::unbind()
	{
		glUseProgram(0);
	}

	bool GlslProgram::get_active() const
	{
		return m_program == get_current_program();
	}

	void GlslProgram::do_build(const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const GlslProgramDescription &description)
	{
		GlslShaderObject vertex(GL_VERTEX_SHADER);
		GlslShaderObject fragment(GL_FRAGMENT_SHADER);

		m_program = glCreateProgram();

		vertex.load(description.get_vertex_shader());
		vertex.compile();
		glAttachShader(get_program(), vertex.get_shader());

		fragment.load(description.get_fragment_shader());
		fragment.compile();
		glAttachShader(get_program(), fragment.get_shader());

		if (!description.get_tess_control_shader().get().empty())
		{
			GlslShaderObject tess_control(GL_TESS_CONTROL_SHADER);

			tess_control.load(
				description.get_tess_control_shader());
			tess_control.compile();

			glAttachShader(get_program(),
				tess_control.get_shader());
		}

		if (!description.get_tess_evaluation_shader().get().empty())
		{
			GlslShaderObject tess_evaluation(
				GL_TESS_EVALUATION_SHADER);

			tess_evaluation.load(
				description.get_tess_evaluation_shader());
			tess_evaluation.compile();

			glAttachShader(get_program(),
				tess_evaluation.get_shader());
		}

		if (!description.get_geometry_shader().get().empty())
		{
			GlslShaderObject geometry(GL_GEOMETRY_SHADER);

			geometry.load(description.get_geometry_shader());
			geometry.compile();

			glAttachShader(get_program(), geometry.get_shader());
		}

		bind_attribute_locations();

/*
		glProgramParameteri(get_program(),
			GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
*/
		glLinkProgram(get_program());
/*
		glGetProgramiv(get_program(), GL_PROGRAM_BINARY_LENGTH, &size);
		SetLength(binary, size);
		glGetProgramBinary(get_program(), size, 0, &format, &binary);
*/

		if (!program_link_status())
		{
			EL_THROW_EXCEPTION(OpenGlException()
				<< errinfo_message(get_program_log()));
		}

		LOG_DEBUG(lt_glsl_program, UTF8("Program linked successful: "
			"%1%"), get_program_log());

		log_attribute_locations();
		log_uniforms(uniform_buffer_description_cache);
	}

	void GlslProgram::build(const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const GlslProgramDescription &description)
	{
		Uint32 i, count;

		LOG_DEBUG(lt_glsl_program, UTF8("Building Shader %1%"),
			get_uuid());

		try
		{
			do_build(uniform_buffer_description_cache, description);
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_parameter_name(
				boost::uuids::to_string(get_uuid()));
			exception << errinfo_vertex_shader_source(
				description.get_vertex_shader());
			exception << errinfo_tess_control_shader_source(
				description.get_tess_control_shader());
			exception << errinfo_tess_evaluation_shader_source(
				description.get_tess_evaluation_shader());
			exception << errinfo_geometry_shader_source(
				description.get_geometry_shader());
			exception << errinfo_fragment_shader_source(
				description.get_fragment_shader());

			throw;
		}

		bind();

		count = SamplerParameterUtil::get_sampler_parameter_count();

		for (i = 0; i < count; ++i)
		{
			set_variant_parameter(SamplerParameterUtil::get_str(
				static_cast<SamplerParameterType>(i)),
					static_cast<Sint64>(i));
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

	void GlslProgram::load_xml(const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const xmlNodePtr node)
	{
		String vertex_shader, tess_control_shader;
		String tess_evaluation_shader, geometry_shader, fragment_shader;
		StringVariantMap values;
		xmlNodePtr it;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("glsl_program")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("vertex_shader")) == 0)
			{
				vertex_shader = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("tess_control_shader")) == 0)
			{
				tess_control_shader =
					XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("tess_evaluation_shader")) == 0)
			{
				tess_evaluation_shader =
					XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("geometry_shader")) == 0)
			{
				geometry_shader = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("fragment_shader")) == 0)
			{
				fragment_shader = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("values")) == 0)
			{
				values = XmlUtil::get_string_variant_map(it,
					UTF8("value"));
			}
		}
		while (XmlUtil::next(it, true));

		build(uniform_buffer_description_cache,	GlslProgramDescription(
				vertex_shader, tess_control_shader,
				tess_evaluation_shader, geometry_shader,
				fragment_shader));
	}

	void GlslProgram::load_xml(const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr reader;

		reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(uniform_buffer_description_cache,
			reader->get_root_node());
	}

	void GlslProgram::load_xml(const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const String &file_name)
	{
		XmlReaderSharedPtr reader;

		reader = XmlReaderSharedPtr(new XmlReader(file_name));

		load_xml(uniform_buffer_description_cache,
			reader->get_root_node());
	}

}
