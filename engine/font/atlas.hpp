/****************************************************************************
 *            atlas.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_583d8c99_2348_4425_a9fb_aacac6bcf4fc
#define	UUID_583d8c99_2348_4425_a9fb_aacac6bcf4fc

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textattribute.hpp"

/**
 * @file
 * @brief The @c class Text.
 * This file contains the @c class Text.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for text.
	 *
	 * @c class for text.
	 */
	class Atlas
	{
		private:
			Uvec3Vector m_nodes;
			const Uint32 m_width;
			const Uint32 m_height;

			Sint32 fit(const Uint32 index, const Uint32 width,
				const Uint32 height) const;
			void merge();

		public:
			/**
			 * Default constructor.
			 */
			Atlas(const Uint32 width, const Uint32 height);

			/**
			 * Default destructor.
			 */
			~Atlas() noexcept;

			void clear();

			bool get_region(const Uint32 width,
				const Uint32 height, glm::uvec2 &offset);

			inline Uint32 get_width() const noexcept
			{
				return m_width;
			}

			inline Uint32 get_height() const noexcept
			{
				return m_height;
			}

	};

}

#endif	/* UUID_583d8c99_2348_4425_a9fb_aacac6bcf4fc */
