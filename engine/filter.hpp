/****************************************************************************
 *            filter.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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

	class Filter: public boost::noncopyable
	{
		private:
			AbstractMeshSharedPtr m_mesh;
			boost::array<GlslProgramSharedPtr, 4> m_programs;
			const Uint16 m_channel_count;

			String get_vertex_str(const Uint16 version) const;
			String get_fragment_str(const Uint16 version,
				const bool layer, const bool vertical) const;

		public:
			Filter(const MeshCacheSharedPtr &mesh_cache,
				const Uint16 channel_count);
			~Filter() throw();
			void bind(const Uint32 width, const Uint32 height,
				const Uint32 layer, const bool vertical,
				StateManager &state_manager);
			void bind(const Uint32 width, const Uint32 height,
				const bool vertical,
				StateManager &state_manager);

			inline Uint16 get_channel_count() const
			{
				return m_channel_count;
			}

	};

}

#endif	/* UUID_59622d83_0a88_4ecf_89f8_0c18abc57dba */
