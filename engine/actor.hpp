/****************************************************************************
 *            actor.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5c4a5a77_94cd_4910_b747_f72d81d37cda
#define	UUID_5c4a5a77_94cd_4910_b747_f72d81d37cda

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "object.hpp"
#include "actortexturebuilder.hpp"

/**
 * @file
 * @brief The @c class Actor.
 * This file contains the @c class Actor.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for actors.
	 *
	 * @c class for actors.
	 */
	class Actor: public Object
	{
		private:
			IndexUpdateSourceSharedPtr m_index_source;
			ActorTextureBuilderSharedPtr m_actor_texture_builder;
			Uint32Set m_meshs;
			BitSet32 m_buffs;

		public:
			/**
			 * Default constructor.
			 */
			Actor(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialDescriptionVector &materials,
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const IndexUpdateSourceSharedPtr &index_source,
				CalCoreModel *core_model);

			/**
			 * Default destructor.
			 */
			~Actor() throw();

			void add_mesh(const Uint32 id);
			void remove_mesh(const Uint32 id);
			void init_enhanced_actor(
				const CodecManagerWeakPtr &codec_manager,
				const FileSystemWeakPtr &file_system,
				const GlobalVarsSharedPtr &global_vars);
			void set_parts(
				const ActorPartTextureTypeStringMap &parts);

			inline BitSet32 get_buffs() const
			{
				return m_buffs;
			}

			inline void set_buffs(const BitSet32 buffs)
			{
				m_buffs = buffs;
			}

	};

}

#endif	/* UUID_5c4a5a77_94cd_4910_b747_f72d81d37cda */
