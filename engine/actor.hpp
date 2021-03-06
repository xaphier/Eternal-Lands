/****************************************************************************
 *            actor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			BitSet64 m_buffs;

		public:
			/**
			 * Default constructor.
			 */
			Actor(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialSharedPtrVector &materials,
				const IndexUpdateSourceSharedPtr &index_source,
				CalCoreModel *core_model);

			/**
			 * Default constructor.
			 */
			Actor(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialSharedPtrVector &materials,
				CalCoreModel *core_model);

			/**
			 * Default destructor.
			 */
			~Actor() noexcept;
			virtual void update_bounding_box();
			virtual void update_bones();
			void add_mesh(const Uint32 id);
			void remove_mesh(const Uint32 id);
			void init_enhanced_actor(
				const FileSystemConstSharedPtr &file_system,
				const GlobalVarsConstSharedPtr &global_vars);
			void set_parts(
				const ActorPartTextureTypeStringMap &parts);

			inline void clear_mesh() noexcept
			{
				m_meshs.clear();
			}

			inline bool get_enhanced_actor() const noexcept
			{
				return m_index_source.get() != nullptr;
			}

			inline BitSet64 get_buffs() const noexcept
			{
				return m_buffs;
			}

			inline void set_buffs(const BitSet64 buffs) noexcept
			{
				m_buffs = buffs;
			}

	};

}

#endif	/* UUID_5c4a5a77_94cd_4910_b747_f72d81d37cda */

