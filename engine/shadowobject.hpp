/****************************************************************************
 *            shadowobject.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c3ca5a3f_8c59_41b5_ac12_1055f32867d9
#define	UUID_c3ca5a3f_8c59_41b5_ac12_1055f32867d9

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "meshdrawdata.hpp"

/**
 * @file
 * @brief The @c class ShadowObject.
 * This file contains the @c class ShadowObject.
 */
namespace eternal_lands
{

	class ShadowObject: public MeshDrawData
	{
		private:
			GlslProgramSharedPtr m_program;
			Uint32 m_mesh_count;

		public:
			/**
			 * Default constructor.
			 */
			ShadowObject();

			/**
			 * Default constructor.
			 */
			ShadowObject(const MeshDrawData &mesh_draw,
				const GlslProgramSharedPtr &program,
				const Uint32 mesh_count);

			/**
			 * Default destructor.
			 */
			~ShadowObject() noexcept;

			inline const GlslProgramSharedPtr &get_program() const
				noexcept
			{
				return m_program;
			}

			inline Uint32 get_mesh_count() const noexcept
			{
				return m_mesh_count;
			}

			inline void set_program(
				const GlslProgramSharedPtr &program) noexcept
			{
				m_program = program;
			}

			inline void set_mesh_count(const Uint32 mesh_count)
				noexcept
			{
				m_mesh_count = mesh_count;
			}

	};

}

#endif	/* UUID_c3ca5a3f_8c59_41b5_ac12_1055f32867d9 */
