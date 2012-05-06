/****************************************************************************
 *            filter.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_59622d83_0a88_4ecf_89f8_0c18abc57dba
#define	UUID_59622d83_0a88_4ecf_89f8_0c18abc57dba

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Filter.
 * This file contains the @c class Filter.
 */
namespace eternal_lands
{

	enum FilterType
	{
		/**
		 * 5 tap gauss filter, uses 3 linear texture fetches.
		 */
		ft_gauss_5_tap,
		/**
		 * 9 tap gauss filter, shorted to use 3 linear texture fetches.
		 */
		ft_gauss_9_tap_short,
		/**
		 * 9 tap gauss filter, uses 5 linear texture fetches.
		 */
		ft_gauss_9_tap,
		/**
		 * 13 tap gauss filter, shorted to use 5 linear texture fetches.
		 */
		ft_gauss_13_tap_short,
		/**
		 * 13 tap gauss filter, uses 7 linear texture fetches.
		 */
		ft_gauss_13_tap,
		/**
		 * 17 tap gauss filter, shorted to use 7 linear texture fetches.
		 */
		ft_gauss_17_tap_short,
		/**
		 * 5 tap box filter, uses 3 linear texture fetches.
		 */
		ft_box_5_tap,
		/**
		 * 9 tap gauss filter, uses 5 linear texture fetches.
		 */
		ft_box_9_tap,
		/**
		 * 13 tap gauss filter, uses 7 linear texture fetches.
		 */
		ft_box_13_tap
	};

	class Filter: public boost::noncopyable
	{
		private:
			AbstractMeshSharedPtr m_mesh;
			boost::array<GlslProgramSharedPtr, 64> m_programs;
			boost::array<GlslProgramSharedPtr, 16>
				m_multisample_programs;

			void build_filter(const StringVariantMap &values,
				const Uint16 version,
				const Uint16 channel_count,
				const Uint16 half_taps_minus_one,
				const bool layer, const bool vertical);
			void build_multisample_filter(
				const StringVariantMap &values,
				const Uint16 version,
				const Uint16 channel_count,
				const Uint16 sample_count);
			static String get_vertex_str(const Uint16 version);
			static String get_fragment_str(const Uint16 version,
				const Uint16 channel_count,
				const Uint16 half_taps_minus_one,
				const bool layer, const bool vertical);
			static Uint16 get_scale_offset(const FilterType type,
				glm::vec4 &scale, glm::vec4 &offset);
			static String get_multisample_vertex_str(
				const Uint16 version);
			static String get_multisample_fragment_str(
				const Uint16 version,
				const Uint16 channel_count,
				const Uint16 sample_count);

			static inline Uint16 get_index(
				const Uint16 channel_count,
				const Uint16 half_taps_minus_one,
				const bool layer, const bool vertical)
			{
				Uint16 result;

				assert(channel_count > 0);
				assert(channel_count < 5);
				assert(half_taps_minus_one < 4);

				result = channel_count - 1;
				result += half_taps_minus_one << 2;

				if (layer)
				{
					result += 1 << 4;
				}

				if (vertical)
				{
					result += 1 << 5;
				}

				return result;
			}

			static inline Uint16 get_multisample_index(
				const Uint16 channel_count,
				const Uint16 sample_count)
			{
				Uint16 result;

				assert(channel_count > 0);
				assert(channel_count < 5);
				assert((sample_count != 2) &&
					(sample_count != 4) &&
					(sample_count != 8) &&
					(sample_count != 16));

				result = channel_count - 1;

				switch (sample_count)
				{
					case 2:
						result += 0 << 2;
						break;
					case 4:
						result += 1 << 2;
						break;
					case 8:
						result += 2 << 2;
						break;
					case 16:
						result += 3 << 2;
						break;
				}

				return result;
			}

		public:
			Filter(const MeshCacheSharedPtr &mesh_cache,
				const GlobalVarsSharedPtr &global_vars);
			~Filter() throw();
			void bind(const glm::vec4 &source,
				const glm::vec4 &dest, const Uint32 width,
				const Uint32 height, const Uint32 layer,
				const Uint16 channel_count,
				const FilterType type, const bool vertical,
				StateManager &state_manager);
			void bind(const glm::vec4 &source,
				const glm::vec4 &dest, const Uint32 width,
				const Uint32 height, const Uint16 channel_count,
				const FilterType type, const bool vertical,
				StateManager &state_manager);
			void bind(const Uint16 channel_count,
				const Uint16 sample_count,
				StateManager &state_manager);

	};

}

#endif	/* UUID_59622d83_0a88_4ecf_89f8_0c18abc57dba */
