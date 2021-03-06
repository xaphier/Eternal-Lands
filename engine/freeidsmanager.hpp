/****************************************************************************
 *            freeidsmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef UUID_733b7c13_b16f_4dc6_b58b_e2e7b28738e6
#define	UUID_733b7c13_b16f_4dc6_b58b_e2e7b28738e6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "freeids.hpp"
#include "exceptions.hpp"

/**
 * @file
 * @brief The @c class FreeIdsManager.
 * This file contains the @c class FreeIdsManager.
 */
namespace eternal_lands
{

	enum IdType
	{
		it_3d_object = 0,
		it_2d_object,
		it_tile_layer_0_object,
		it_tile_layer_1_object,
		it_tile_layer_2_object,
		it_tile_layer_3_object,
		it_dynamic_object,
		it_light_object,
		it_particle_object
	};

	/**
	 * @brief @c class for free ids.
	 *
	 * @c class for free ids.
	 */
	class FreeIdsManager
	{
		private:
			boost::array<FreeIds, 9> m_free_ids;
			FreeIds m_free_light_ids;
			FreeIds m_free_particle_ids;
			FreeIds m_free_decal_ids;

		public:
			/**
			 * Default constructor.
			 */
			FreeIdsManager();

			/**
			 * Default destructor.
			 */
			~FreeIdsManager() noexcept;
			void clear();
			Uint32 get_next_free_object_id(const IdType type);
			void free_object_id(const Uint32 id);
			Uint32 use_typeless_object_id(const Uint32 typeless_id,
				const IdType type);
			void use_object_id(const Uint32 id);
			Uint32 get_object_id(const Uint32 typeless_id,
				const IdType type);
			Uint32 get_next_free_light_id();
			void free_light_id(const Uint32 id);
			void use_light_id(const Uint32 id);
			Uint32 get_next_free_particle_id();
			void free_particle_id(const Uint32 id);
			void use_particle_id(const Uint32 id);
			Uint32 get_next_free_decal_id();
			void free_decal_id(const Uint32 id);
			void use_decal_id(const Uint32 id);
			bool get_is_object_id_free(const Uint32 id) const;
			bool get_is_light_id_free(const Uint32 id) const;
			bool get_is_particle_id_free(const Uint32 id) const;
			bool get_is_decal_id_free(const Uint32 id) const;
			Uint32 get_tile_id(const Uint16 x, const Uint16 y,
				const Uint16 layer);

			FreeIds &get_free_object_ids(const IdType type)
			{
				return m_free_ids[type];
			}

			static inline bool get_id_type(const Uint32 id,
				Uint32 &typeless_id, IdType &type)
			{
				Uint16 tmp;

				tmp = FreeIds::get_type_value(id);

				if (tmp >= it_particle_object)
				{
					return false;
				}

				typeless_id = FreeIds::get_typeless_id(id);

				type = static_cast<IdType>(tmp);

				return true;
			}

	};

}

#endif	/* UUID_733b7c13_b16f_4dc6_b58b_e2e7b28738e6 */
