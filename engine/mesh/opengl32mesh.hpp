/****************************************************************************
 *            opengl31mesh.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_cfbf3d73_d078_4803_ab3c_bdc9507a04c8
#define	UUID_cfbf3d73_d078_4803_ab3c_bdc9507a04c8

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "opengl3mesh.hpp"

/**
 * @file
 * @brief The @c class OpenGl3Mesh.
 * This file contains the @c class OpenGl3Mesh.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for meshs in a format for OpenGL 3.2.
	 *
	 * @c class for meshs in a format for OpenGL 3.2 using hardware
	 * buffers, shaders and instancing.
	 */
	class OpenGl32Mesh: public OpenGl3Mesh
	{
		private:

		public:
			/**
			 * Default constructor.
			 */
			OpenGl32Mesh(const String &name,
				const bool static_indices,
				const bool static_vertices,
				const bool use_simd);

			/**
			 * Default destructor.
			 */
			virtual ~OpenGl32Mesh() throw();
			virtual AbstractMeshSharedPtr clone_vertex_data() const;
			virtual AbstractMeshSharedPtr clone_index_data() const;
			virtual bool get_supports_base_vertex() const;

			/**
			 * Draws the mesh using the given draw data.
			 */
			virtual void draw(const MeshDrawData &draw_data,
				const Uint32 instances);

	};

}

#endif	/* UUID_cfbf3d73_d078_4803_ab3c_bdc9507a04c8 */