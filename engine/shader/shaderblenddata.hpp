/****************************************************************************
 *            shaderblenddata.hpp
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
#include "shaderblendutil.hpp"

/**
 * @file
 * @brief The @c class ShaderBlendData.
 * This file contains the @c class ShaderBlendData.
 */
namespace eternal_lands
{

	class ShaderBlendData
	{
		private:
			/**
			 * scale & offset for height based blending,
			 * blend size else.
			 */
			glm::vec2 m_data;
			ShaderBlendType m_blend;

		public:
			ShaderBlendData();
			ShaderBlendData(const glm::vec2 &data,
				const ShaderBlendType blend);
			ShaderBlendData(const ShaderBlendType blend);
			ShaderBlendData(const xmlNodePtr node);
			~ShaderBlendData() noexcept;
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;

			inline bool operator==(
				const ShaderBlendData &blend_data) const
			{
				return	(blend_data.m_data == m_data) &&
					(blend_data.m_blend == m_blend);
			}

			inline const glm::vec2 &get_data() const noexcept
			{
				return m_data;
			}

			inline ShaderBlendType get_blend() const noexcept
			{
				return m_blend;
			}

			inline void set_data(const glm::vec2 &data) noexcept
			{
				m_data = data;
			}

			inline void set_blend(const ShaderBlendType blend)
				noexcept
			{
				m_blend = blend;
			}

	};

}

#endif	/* UUID_ad4d31b2_6011_47c0_84a8_3daa9fdcc0aa */
