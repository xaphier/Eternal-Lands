/****************************************************************************
 *            xmlwriter.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a08cebb5_7815_4eae_a24f_7870891f66ec
#define	UUID_a08cebb5_7815_4eae_a24f_7870891f66ec

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class XmlWriter.
 * This file contains the @c class XmlWriter.
 */
namespace eternal_lands
{

	class XmlWriter: public boost::noncopyable
	{
		private:
			const XmlBufferSharedPtr m_buffer;
			xmlTextWriterPtr m_writer;

		public:
			XmlWriter(const XmlBufferSharedPtr &buffer);
			XmlWriter(const String &file_name);
			~XmlWriter() noexcept;
			void start_element(const String &name);
			void end_element();
			void write_element(const String &name,
				const String &value);
			void write_bitset64_element(const String &name,
				const BitSet64 &value);
			void write_uuid_element(const String &name,
				const boost::uuids::uuid &value);
			void write_bool_element(const String &name,
				const bool value);
			void write_float_element(const String &name,
				const float value);
			void write_int_element(const String &name,
				const Sint64 value);
			void write_bvec2_element(const String &name,
				const glm::bvec2 &value);
			void write_bvec3_element(const String &name,
				const glm::bvec3 &value);
			void write_bvec4_element(const String &name,
				const glm::bvec4 &value);
			void write_uvec2_element(const String &name,
				const glm::uvec2 &value);
			void write_uvec3_element(const String &name,
				const glm::uvec3 &value);
			void write_uvec4_element(const String &name,
				const glm::uvec4 &value);
			void write_ivec2_element(const String &name,
				const glm::ivec2 &value);
			void write_ivec3_element(const String &name,
				const glm::ivec3 &value);
			void write_ivec4_element(const String &name,
				const glm::ivec4 &value);
			void write_vec2_element(const String &name,
				const glm::vec2 &value);
			void write_vec3_element(const String &name,
				const glm::vec3 &value);
			void write_vec4_element(const String &name,
				const glm::vec4 &value);
			void write_mat2_element(const String &name,
				const glm::mat2 &value);
			void write_mat2x3_element(const String &name,
				const glm::mat2x3 &value);
			void write_mat2x4_element(const String &name,
				const glm::mat2x4 &value);
			void write_mat3x2_element(const String &name,
				const glm::mat3x2 &value);
			void write_mat3_element(const String &name,
				const glm::mat3 &value);
			void write_mat3x4_element(const String &name,
				const glm::mat3x4 &value);
			void write_mat4x2_element(const String &name,
				const glm::mat4x2 &value);
			void write_mat4x3_element(const String &name,
				const glm::mat4x3 &value);
			void write_mat4_element(const String &name,
				const glm::mat4 &value);
			void write_quaternion_element(const String &name,
				const glm::quat &value);
			void write_variant_element(const String &element,
				const Variant &variant);
			void write_string_variant_element(
				const String &element,
				const String &name,
				const Variant &variant);
			void write_string_variant_map(const String &element,
				const String &elements,
				const StringVariantMap &variants);
			void write_string(const String &value);
			void write_property(const String &property,
				const String &value);
			void write_bool_property(const String &property,
				const bool value);
			void write_float_vector_element(const String &name,
				const FloatVector &values);
			void write_s16_vector_element(const String &name,
				const Sint16Vector &values);
			void write_s32_vector_element(const String &name,
				const Sint32Vector &values);
			void write_u16_vector_element(const String &name,
				const Uint16Vector &values);
			void write_u32_vector_element(const String &name,
				const Uint32Vector &values);

	};

}

#endif	/* UUID_a08cebb5_7815_4eae_a24f_7870891f66ec */
