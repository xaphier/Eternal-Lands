/****************************************************************************
 *	    glslprogram.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef UUID_01d89c42_b82a_48c3_aec5_fa945e6affec
#define UUID_01d89c42_b82a_48c3_aec5_fa945e6affec

#ifndef __cplusplus
#error  "Including C++ header in C translation unit!"
#endif  /* __cplusplus */

#include "prerequisites.hpp"
#include "vertexelement.hpp"
#include "shader/autoparameterutil.hpp"

/**
 * @file
 * @brief The @c class GlslProgram.
 * This file contains the @c class GlslProgram.
 */
namespace eternal_lands
{

	class GlslProgram: public boost::noncopyable
	{
		private:
			class Uniform;
			typedef std::map<String, Uniform> StringUniformMap;
			typedef boost::unordered_map<AutoParameterType,
				Uniform> AutoParameterTypeUniformMap;
			typedef std::map<String, Uint16> StringUint16Map;

			StringUniformMap m_uniforms;
			AutoParameterTypeUniformMap m_auto_parameters;
			StringUint16Map m_samplers;
			String m_name;
			Uint64 m_last_used;
			GLuint m_program;
			BitSet32 m_used_texture_units;
			BitSet32 m_used_attributes;

			void bind_attribute_location(
				const VertexSemanticType type);
			void bind_attribute_locations();
			bool get_active() const;
			static GLuint get_current_program();
			StringType get_program_log();
			bool program_link_status();
			bool program_validate_status();
			void log_attribute_locations();
			void log_uniforms();
			void do_build(const StringType &vertex_shader,
				const StringType &geometry_shader,
				const StringType &fragment_shader);
			void build(const StringType &vertex_shader,
				const StringType &geometry_shader,
				const StringType &fragment_shader,
				const StringVariantMap &values);
			void update_used_texture_units();
			void load_xml(const xmlNodePtr node);
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			void load_xml(const String &file_name);

			inline GLuint get_program() const
			{
				return m_program;
			}

		public:
			GlslProgram(const StringType &vertex_shader,
				const StringType &geometry_shader,
				const StringType &fragment_shader,
				const StringVariantMap &values,
				const String &name);
			GlslProgram(const FileSystemSharedPtr &file_system,
				const String &file_name);
			GlslProgram(const String &file_name);
			~GlslProgram() throw();
			void bind();
			void set_variant_parameter(const String &name,
				const Variant &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const bool value);
			void set_parameter(const AutoParameterType parameter,
				const glm::bvec2 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::bvec3 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::bvec4 &value);
			void set_parameter(const AutoParameterType parameter,
				const Sint32 value);
			void set_parameter(const AutoParameterType parameter,
				const glm::ivec2 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::ivec3 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::ivec4 &value);
			void set_parameter(const AutoParameterType parameter,
				const Sint32Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const Ivec2Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const Ivec3Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const Ivec4Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const Uint32 value);
			void set_parameter(const AutoParameterType parameter,
				const glm::uvec2 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::uvec3 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::uvec4 &value);
			void set_parameter(const AutoParameterType parameter,
				const Uint32Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const Uvec2Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const Uvec3Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const Uvec4Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const float value);
			void set_parameter(const AutoParameterType parameter,
				const FloatVector &value);
			void set_parameter(const AutoParameterType parameter,
				const Vec2Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const Vec3Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const Vec4Vector &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::vec2 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::vec3 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::vec4 &value);
			void set_parameter(const AutoParameterType parameter,
				const Mat2x2Vector &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat2x3Vector &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat2x4Vector &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat3x2Vector &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat3x3Vector &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat3x4Vector &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat4x2Vector &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat4x3Vector &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat4x4Vector &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat2x2 &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat2x3 &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat2x4 &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat3x2 &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat3x3 &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat3x4 &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat4x2 &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat4x3 &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat4x4 &value,
				const bool transpose = false);
			void check();
			bool validate();
			void log_validate_status();
			void unbind();

			inline void set_bool_parameter(
				const AutoParameterType parameter,
				const bool value)
			{
				set_parameter(parameter, value);
			}

			inline void set_int_parameter(
				const AutoParameterType parameter,
				const Sint32 value)
			{
				set_parameter(parameter, value);
			}

			inline void set_uint_parameter(
				const AutoParameterType parameter,
				const Uint32 value)
			{
				set_parameter(parameter, value);
			}

			inline void set_float_parameter(
				const AutoParameterType parameter,
				const float value)
			{
				set_parameter(parameter, value);
			}

			inline const String &get_name() const
			{
				return m_name;
			}

			inline void set_last_used(const Uint64 last_used)
			{
				m_last_used = last_used;
			}

			inline Uint64 get_last_used() const
			{
				return m_last_used;
			}

			inline BitSet32 get_used_texture_units() const
			{
				return m_used_texture_units;
			}

			inline BitSet32 get_used_attributes() const
			{
				return m_used_attributes;
			}

	};

}

#endif  /* UUID_01d89c42_b82a_48c3_aec5_fa945e6affec */
