/****************************************************************************
 *            e2dloader.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_61afc624_7257_43dc_bbc5_03daa07fae12
#define	UUID_61afc624_7257_43dc_bbc5_03daa07fae12

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class E2dLoader.
 * This file contains the @c class E2dLoader.
 */
namespace eternal_lands
{

	/**
	 * @brief @c Class for loading e2d files.
	 *
	 * @c Class E2dLoader is used for loading e2d files.
	 * @see MeshCache
	 */
	class E2dLoader
	{
		private:
			ReaderSharedPtr m_reader;

			void load_vertex_data(
				const MeshDataToolSharedPtr &mesh_data_tool,
				const glm::vec4 &texture_coordinates,
				const glm::vec2 &size);

			static inline Uint16 get_tile_size()
			{
				return 4;
			}

		public:
			E2dLoader(const ReaderSharedPtr &reader);
			~E2dLoader() throw();
			void load(const MaterialDescriptionCacheSharedPtr
					&material_description_cache,
				const bool use_simd,
				MeshDataToolSharedPtr &mesh_data_tool,
				MaterialEffectDescriptionVector &materials);
			static bool check_format(const Uint8Array8 &id);

	};

}

#endif	/* UUID_61afc624_7257_43dc_bbc5_03daa07fae12 */

