/****************************************************************************
 *            walkheightbuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_95303102_90ea_4882_ac01_a4c02aa791da
#define	UUID_95303102_90ea_4882_ac01_a4c02aa791da

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class WalkHeightBuilder.
 * This file contains the @c class WalkHeightBuilder.
 */
namespace eternal_lands
{

	class WalkHeightBuilder
	{
		private:
			const GlobalVarsConstSharedPtr m_global_vars;
			const MeshBuilderConstWeakPtr m_mesh_builder;

			inline GlobalVarsConstSharedPtr get_global_vars() const
				noexcept
			{
				return m_global_vars;
			}

			inline MeshBuilderConstSharedPtr get_mesh_builder() const
				noexcept
			{
				MeshBuilderConstSharedPtr result;

				result = m_mesh_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

		public:
			/**
			 * Default constructor.
			 */
			WalkHeightBuilder(
				const GlobalVarsConstSharedPtr &global_vars,
				const MeshBuilderConstSharedPtr &mesh_builder);

			/**
			 * Default destructor.
			 */
			~WalkHeightBuilder() noexcept;

			bool get_walk_height(
				const Uint16MultiArray2 &walk_height_page,
				AbstractMeshSharedPtr &mesh) const;

			static inline Uint32 get_walk_height_size() noexcept
			{
				return 16;
			}

			static inline float get_walk_height_world_scale()
				noexcept
			{
				return 0.5f;
			}

			static inline float get_walk_height_world_scale_z()
				noexcept
			{
				return 0.2f;
			}

			static inline float get_walk_height_world_offset_z()
				noexcept
			{
				return -2.2f;
			}

	};

}

#endif	/* UUID_95303102_90ea_4882_ac01_a4c02aa791da */
