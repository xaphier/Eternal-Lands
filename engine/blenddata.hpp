/****************************************************************************
 *            blenddata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ad4d31b2_6011_47c0_84a8_3daa9fdcc0aa
#define	UUID_ad4d31b2_6011_47c0_84a8_3daa9fdcc0aa

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class BlendData.
 * This file contains the @c class BlendData.
 */
namespace eternal_lands
{

	class BlendData
	{
		private:
			float m_blend_size;
			bool m_use_blend_size;

		public:
			BlendData();
			BlendData(const float blend_size);
			BlendData(const float blend_size,
				const bool use_blend_size);
			BlendData(const xmlNodePtr node);
			~BlendData() noexcept;
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;
			bool operator==(const BlendData &blend_data)
				const;

			inline float get_blend_size() const noexcept
			{
				return m_blend_size;
			}

			inline bool get_use_blend_size() const noexcept
			{
				return m_use_blend_size;
			}

			inline void set_blend_size(const float blend_size)
				noexcept
			{
				m_blend_size = blend_size;
			}

			inline void set_use_blend_size(
				const bool use_blend_size) noexcept
			{
				m_use_blend_size = use_blend_size;
			}

	};

}

#endif	/* UUID_ad4d31b2_6011_47c0_84a8_3daa9fdcc0aa */
