/****************************************************************************
 *            materialeffectdescription.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8e76c7db_bc4f_4097_bd23_d576c8714bd5
#define	UUID_8e76c7db_bc4f_4097_bd23_d576c8714bd5

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "materialdescription.hpp"

/**
 * @file
 * @brief The @c class MaterialEffectDescription.
 * This file contains the @c class MaterialEffectDescription.
 */
namespace eternal_lands
{

	class MaterialEffectDescription: public MaterialDescription
	{
		private:
			String m_world_transformation;
			bool m_cast_shadows;
			bool m_culling;
			bool m_transparent;
			bool m_billboard;

		public:
			MaterialEffectDescription();
			~MaterialEffectDescription() throw();
			bool operator==(
				const MaterialEffectDescription &material)
				const;
			bool operator!=(
				const MaterialEffectDescription &material)
				const;
			bool operator<(
				const MaterialEffectDescription &material)
				const;
			void set_material_descriptiont(
				const MaterialDescription &material);
			void set_effect_description(
				const EffectDescription &effect);
			void get_effect_description(EffectDescription &effect)
				const;

			inline const String &get_world_transformation() const
			{
				return m_world_transformation;
			}

			inline bool get_cast_shadows() const
			{
				return m_cast_shadows;
			}

			inline bool get_culling() const
			{
				return m_culling;
			}

			inline bool get_transparent() const
			{
				return m_transparent;
			}

			inline bool get_billboard() const
			{
				return m_billboard;
			}

			inline void set_world_transformation(
				const String &world_transformation)
			{
				m_world_transformation = world_transformation;
			}

			inline void set_cast_shadows(const bool cast_shadows)
			{
				m_cast_shadows = cast_shadows;
			}

			inline void set_culling(const bool culling)
			{
				m_culling = culling;
			}

			inline void set_transparent(const bool transparent)
			{
				m_transparent = transparent;
			}

			inline void set_billboard(const bool billboard)
			{
				m_billboard = billboard;
			}

	};

	OutStream& operator<<(OutStream &str,
		const MaterialEffectDescription &value);

}

#endif	/* UUID_8e76c7db_bc4f_4097_bd23_d576c8714bd5 */
