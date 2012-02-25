/****************************************************************************
 *            xmlwriter.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			xmlTextWriterPtr m_writer;

		public:
			XmlWriter(xmlBufferPtr buffer);
			XmlWriter(const String &file_name);
			~XmlWriter() throw();
			void start_element(const StringType &name);
			void end_element();
			void write_element(const StringType &name,
				const StringType &value);
			void write_bool_element(const StringType &name,
				const bool value);
			void write_float_element(const StringType &name,
				const float value);
			void write_int_element(const StringType &name,
				const Sint64 value);
			void write_vec2_element(const StringType &name,
				const glm::vec2 &value);
			void write_vec3_element(const StringType &name,
				const glm::vec3 &value);
			void write_vec4_element(const StringType &name,
				const glm::vec4 &value);
			void write_mat2_element(const StringType &name,
				const glm::mat2 &value);
			void write_mat2x3_element(const StringType &name,
				const glm::mat2x3 &value);
			void write_mat2x4_element(const StringType &name,
				const glm::mat2x4 &value);
			void write_mat3x2_element(const StringType &name,
				const glm::mat3x2 &value);
			void write_mat3_element(const StringType &name,
				const glm::mat3 &value);
			void write_mat3x4_element(const StringType &name,
				const glm::mat3x4 &value);
			void write_mat4x2_element(const StringType &name,
				const glm::mat4x2 &value);
			void write_mat4x3_element(const StringType &name,
				const glm::mat4x3 &value);
			void write_mat4_element(const StringType &name,
				const glm::mat4 &value);
			void write_quaternion_element(const StringType &name,
				const glm::quat &value);
			void write_variant_element(const StringType &element,
				const Variant &variant);
			void write_string_variant_element(
				const StringType &element,
				const StringType &name,
				const Variant &variant);
			void write_string_variant_map(const StringType &element,
				const StringType &elements,
				const StringVariantMap &variants);

	};

}

#endif	/* UUID_a08cebb5_7815_4eae_a24f_7870891f66ec */
