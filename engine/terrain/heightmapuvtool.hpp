/****************************************************************************
 *            heightmapuvtool.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c4a6d8e9_ab8a_4e83_8157_46218039aa41
#define	UUID_c4a6d8e9_ab8a_4e83_8157_46218039aa41

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class HeightMapTool.
 * This file contains the @c class HeightMapTool.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for height map.
	 *
	 * @c class for height map.
	 */
	class HeightMapUvTool
	{
		private:
			class Info;
			typedef std::vector<Info> InfoVector;

			Vec2Vector m_uvs;
			Vec2Vector m_velocities;
			InfoVector m_infos;
			Uint32 m_width, m_height;

			static float relax(const InfoVector &infos,
				const float damping, const float clamping,
				const Uint32 line, Vec2Vector &uvs,
				Vec2Vector &velocities);			
			void build_data(const ImageSharedPtr &height_map,
				const float height_scale);

		public:
			/**
			 * Default constructor.
			 */
			HeightMapUvTool(const ImageSharedPtr &height_map,
				const float height_scale);

			/**
			 * Default destructor.
			 */
			~HeightMapUvTool() throw();

			void buil_relaxed_uv();
			void convert();

			inline const glm::vec2 &get_uv(const Uint16 x,
				const Uint16 y) const
			{
				assert(x < m_width);
				assert(y < m_height);

				return m_uvs[x + y * m_width];
			}

	};

}

#endif	/* UUID_c4a6d8e9_ab8a_4e83_8157_46218039aa41 */
