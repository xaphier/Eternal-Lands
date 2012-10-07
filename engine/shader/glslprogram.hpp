/****************************************************************************
 *	    glslprogram.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			typedef std::map<AutoParameterType,
				Uniform> AutoParameterTypeUniformMap;
			typedef std::map<String, Uint16> StringUint16Map;

			StringUniformMap m_uniforms;
			AutoParameterTypeUniformMap m_auto_parameters;
			StringUint16Map m_samplers;
			const boost::uuids::uuid m_uuid;
			Uint64 m_last_used;
			GLuint m_program;
			BitSet32 m_used_texture_units;
			BitSet32 m_used_attributes;

			void bind_attribute_location(
				const VertexSemanticType type);
			void bind_attribute_locations();
			bool get_active() const;
			static GLuint get_current_program();
			String get_program_log();
			bool program_link_status();
			bool program_validate_status();
			void log_attribute_locations();
			void check_uniform_buffer(
				const UniformBufferDescriptionCacheSharedPtr
					&uniform_buffer_description_cache,
				const Uint32 index);
			void log_uniforms(
				const UniformBufferDescriptionCacheSharedPtr
					&uniform_buffer_description_cache);
			void do_build(
				const UniformBufferDescriptionCacheSharedPtr
					&uniform_buffer_description_cache,
				const GlslProgramDescription &description);
			void build(const UniformBufferDescriptionCacheSharedPtr
					&uniform_buffer_description_cache,
				const GlslProgramDescription &description);
			void update_used_texture_units();
			void load_xml(
				const UniformBufferDescriptionCacheSharedPtr
					&uniform_buffer_description_cache,
				const xmlNodePtr node);
			void load_xml(
				const UniformBufferDescriptionCacheSharedPtr
					&uniform_buffer_description_cache,
				const FileSystemSharedPtr &file_system,
				const String &file_name);

			inline GLuint get_program() const noexcept
			{
				return m_program;
			}

		public:
			GlslProgram(const UniformBufferDescriptionCacheSharedPtr
					&uniform_buffer_description_cache,
				const GlslProgramDescription &description,
				const boost::uuids::uuid &uuid);
			GlslProgram(const FileSystemSharedPtr &file_system,
				const UniformBufferDescriptionCacheSharedPtr
					&uniform_buffer_description_cache,
				const String &file_name,
				const boost::uuids::uuid &uuid);
			~GlslProgram() noexcept;
			void bind();
			void set_variant_parameter(const String &name,
				const Variant &value, const Uint32 offset = 0,
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
				const Sint32Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Ivec2Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Ivec3Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Ivec4Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Uint32 value);
			void set_parameter(const AutoParameterType parameter,
				const glm::uvec2 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::uvec3 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::uvec4 &value);
			void set_parameter(const AutoParameterType parameter,
				const Uint32Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Uvec2Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Uvec3Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Uvec4Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const float value);
			void set_parameter(const AutoParameterType parameter,
				const FloatVector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Vec2Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Vec3Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Vec4Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::vec2 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::vec3 &value);
			void set_parameter(const AutoParameterType parameter,
				const glm::vec4 &value);
			void set_parameter(const AutoParameterType parameter,
				const Mat2x2Vector &value,
				const Uint32 offset = 0,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat2x3Vector &value,
				const Uint32 offset = 0,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat2x4Vector &value,
				const Uint32 offset = 0,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat3x2Vector &value,
				const Uint32 offset = 0,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat3x3Vector &value,
				const Uint32 offset = 0,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat3x4Vector &value,
				const Uint32 offset = 0,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat4x2Vector &value,
				const Uint32 offset = 0,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat4x3Vector &value,
				const Uint32 offset = 0,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat4x4Vector &value,
				const Uint32 offset = 0,
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
			void set_parameter(const AutoParameterType parameter,
				const Mat2x3Array2 &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Mat2x4Array4 &value,
				const bool transpose = false);
			void set_parameter(const AutoParameterType parameter,
				const Vec4Array4 &value);
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

			inline const boost::uuids::uuid &get_uuid() const
				noexcept
			{
				return m_uuid;
			}

			inline void set_last_used(const Uint64 last_used)
				noexcept
			{
				m_last_used = last_used;
			}

			inline Uint64 get_last_used() const noexcept
			{
				return m_last_used;
			}

			inline BitSet32 get_used_texture_units() const noexcept
			{
				return m_used_texture_units;
			}

			inline BitSet32 get_used_attributes() const noexcept
			{
				return m_used_attributes;
			}

	};

}

#endif  /* UUID_01d89c42_b82a_48c3_aec5_fa945e6affec */
