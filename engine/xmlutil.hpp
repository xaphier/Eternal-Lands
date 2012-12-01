/****************************************************************************
 *            xmlutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			static boost::uuids::uuid get_uuid_value(
				const xmlNodePtr node);
			static glm::bvec2 get_bvec2_value(
				const xmlNodePtr node);
			static glm::bvec3 get_bvec3_value(
				const xmlNodePtr node);
			static glm::bvec4 get_bvec4_value(
				const xmlNodePtr node);
			static glm::uvec2 get_uvec2_value(
				const xmlNodePtr node);
			static glm::uvec3 get_uvec3_value(
				const xmlNodePtr node);
			static glm::uvec4 get_uvec4_value(
				const xmlNodePtr node);
			static glm::ivec2 get_ivec2_value(
				const xmlNodePtr node);
			static glm::ivec3 get_ivec3_value(
				const xmlNodePtr node);
			static glm::ivec4 get_ivec4_value(
				const xmlNodePtr node);
			static glm::vec2 get_vec2_value(const xmlNodePtr node);
			static glm::vec3 get_vec3_value(const xmlNodePtr node);
			static glm::vec4 get_vec4_value(const xmlNodePtr node);
			static glm::quat get_quaternion_value(
				const xmlNodePtr node);
			static glm::mat2 get_mat2_value(const xmlNodePtr node);
			static glm::mat2x3 get_mat2x3_value(
				const xmlNodePtr node);
			static glm::mat2x4 get_mat2x4_value(
				const xmlNodePtr node);
			static glm::mat3x2 get_mat3x2_value(
				const xmlNodePtr node);
			static glm::mat3 get_mat3_value(const xmlNodePtr node);
			static glm::mat3x4 get_mat3x4_value(
				const xmlNodePtr node);
			static glm::mat4x2 get_mat4x2_value(
				const xmlNodePtr node);
			static glm::mat4x3 get_mat4x3_value(
				const xmlNodePtr node);
			static glm::mat4 get_mat4_value(const xmlNodePtr node);
			static float get_float_value(const xmlNodePtr node);
			static Sint32 get_s32_value(const xmlNodePtr node);
			static Uint32 get_u32_value(const xmlNodePtr node);
			static Sint16 get_s16_value(const xmlNodePtr node);
			static Uint16 get_u16_value(const xmlNodePtr node);
			static String get_string_value(const xmlNodePtr node);
			static BitSet64 get_bitset64_value(
				const xmlNodePtr node);
			static Variant get_variant_value(const xmlNodePtr node);
			static StringVariantPair get_string_variant(
				const xmlNodePtr node);
			static StringVariantMap get_string_variant_map(
				const xmlNodePtr node,
				const String &element);
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
			static bool get_bool_property(const xmlNodePtr node,
				const String &property);
			static FloatVector get_float_vector(
				const xmlNodePtr node);
			static Sint32Vector get_s32_vector(
				const xmlNodePtr node);
			static Sint16Vector get_s16_vector(
				const xmlNodePtr node);
			static Uint32Vector get_u32_vector(
				const xmlNodePtr node);
			static Uint16Vector get_u16_vector(
				const xmlNodePtr node);

	};

}

#endif	/* UUID_ed53622b_159a_440c_887b_10019a5813a6 */
