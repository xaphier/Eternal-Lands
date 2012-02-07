/****************************************************************************
 *            xmlutil.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ed53622b_159a_440c_887b_10019a5813a6
#define	UUID_ed53622b_159a_440c_887b_10019a5813a6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class XmlUtil.
 * This file contains the @c class XmlUtil.
 */
namespace eternal_lands
{

	class XmlUtil
	{
		public:
			static glm::vec2 get_vec2_value(const xmlNodePtr node);
			static glm::vec3 get_vec3_value(const xmlNodePtr node);
			static glm::vec4 get_vec4_value(const xmlNodePtr node);
			static glm::quat get_quaternion_value(
				const xmlNodePtr node);
			static glm::mat2x4 get_mat2x4_value(
				const xmlNodePtr node);
			static glm::mat3x4 get_mat3x4_value(
				const xmlNodePtr node);
			static glm::mat4 get_mat4_value(const xmlNodePtr node);
			static float get_float_value(const xmlNodePtr node);
			static Sint32 get_sint32_value(const xmlNodePtr node);
			static Uint32 get_uint32_value(const xmlNodePtr node);
			static Sint16 get_sint16_value(const xmlNodePtr node);
			static Uint16 get_uint16_value(const xmlNodePtr node);
			static String get_string_value(const xmlNodePtr node);
			static Variant get_variant_value(const xmlNodePtr node);
			static StringVariantPair get_string_variant(
				const xmlNodePtr node);
			static StringVariantMap get_string_variant_map(
				const xmlNodePtr node,
				const StringType &element);
			static bool get_bool_value(const xmlNodePtr node);
			static bool skip_non_elements(xmlNodePtr &node);
			static void forece_next(xmlNodePtr &node,
				const bool elements_only);
			static bool next(xmlNodePtr &node,
				const bool elements_only);
			static xmlNodePtr children(const xmlNodePtr node,
				const bool elements_only);
			static bool has_children(const xmlNodePtr node,
				const bool elements_only);

	};

}

#endif	/* UUID_ed53622b_159a_440c_887b_10019a5813a6 */
