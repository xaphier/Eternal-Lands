/****************************************************************************
 *            opengl31mesh.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_44cee017_6c76_4f62_ba04_4752efb0a6b6
#define	UUID_44cee017_6c76_4f62_ba04_4752efb0a6b6

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
	 * @brief @c class for meshs in a format for OpenGL 3.1.
	 *
	 * @c class for meshs in a format for OpenGL 3.1 using hardware
	 * buffers, shaders and instancing.
	 */
	class OpenGl31Mesh: public OpenGl3Mesh
	{
		private:

		public:
			/**
			 * Default constructor.
			 */
			OpenGl31Mesh(const String &name,
				const bool static_indices,
				const bool static_vertices,
				const bool use_simd);

			/**
			 * Default destructor.
			 */
			virtual ~OpenGl31Mesh() throw();

			virtual void bind(BitSet32 &used_attributes);
			virtual void unbind();
			virtual AbstractMeshSharedPtr clone_vertex_data() const;
			virtual AbstractMeshSharedPtr clone_index_data() const;
			virtual bool get_supports_restart_index();

	};

}

#endif	/* UUID_44cee017_6c76_4f62_ba04_4752efb0a6b6 */
