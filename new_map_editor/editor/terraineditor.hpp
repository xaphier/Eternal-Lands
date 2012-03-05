/****************************************************************************
 *            terraineditor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_02d7abde_d68e_4b22_ac51_99a9c5f4463c
#define	UUID_02d7abde_d68e_4b22_ac51_99a9c5f4463c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "texture.hpp"
#include "imagevalue.hpp"
#include "height.hpp"

namespace eternal_lands
{

	enum EditorBrushType
	{
		ebt_set = 0,
		ebt_const = 1,
		ebt_linear = 2,
		ebt_quadratic = 3,
		ebt_linear_smooth = 4,
		ebt_quadratic_smooth = 5
	};

	class TerrainEditor
	{
		protected:
			ImageSharedPtr m_blend_image;
			TextureSharedPtr m_texture;

			static void get_heights(const Uint16Array2 &vertex,
				const float radius, HeightVector &heights);
			static void change_heights(const Uint16Array2 &vertex,
				const float strength, const float radius,
				const EditorBrushType brush_type,
				HeightVector &heights);
			void get_blend_values(const Uint16Array2 &vertex,
				const float radius,
				ImageValueVector &blend_values) const;
			static void change_blend_values(
				const Uint16Array2 &position,
				const Uint32 index, const float strength,
				const float radius,
				const EditorBrushType brush_type,
				ImageValueVector &blend_values);
			static float calc_brush_effect(const glm::vec2 &centre,
				const glm::vec2 &point, const float value,
				const float average, const float strength,
				const float radius,
				const EditorBrushType brush_type);
			static float get_blend_value(const glm::vec4 &blend,
				const Uint32 index);
			static void set_blend_value(const float value,
				const Uint32 index, glm::vec4 &blend);
			static EditorBrushType get_brush_type(
				const int brush_type);
			static glm::vec4 get_blend_values(
				const glm::vec4 &blend);

		public:
			const String get_blend_image_name() const;
			void set_blend_image_name(const String &blend_image);

	};

}

#endif	/* UUID_02d7abde_d68e_4b22_ac51_99a9c5f4463c */
