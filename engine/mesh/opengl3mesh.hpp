/****************************************************************************
 *            opengl3mesh.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ed58a002_ed47_43c1_acda_0bd30dbb9d7a
#define	UUID_ed58a002_ed47_43c1_acda_0bd30dbb9d7a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "opengl2mesh.hpp"

/**
 * @file
 * @brief The @c class OpenGl3Mesh.
 * This file contains the @c class OpenGl3Mesh.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for meshs in a format for OpenGL 3.x.
	 *
	 * @c class for meshs in a format for OpenGL 3.x using hardware
	 * buffers, shaders and instancing.
	 */
	class OpenGl3Mesh: public OpenGl2Mesh
	{
		private:
			class VertexArrayObject;
			/**
			 * Vertex array object
			 */
			boost::shared_ptr<VertexArrayObject> m_id;
			BitSet32 m_used_attributes;

		protected:
			virtual void init_vertices();

		public:
			/**
			 * Default constructor.
			 */
			OpenGl3Mesh(const String &name,
				const bool static_indices,
				const bool static_vertices);

			/**
			 * Default destructor.
			 */
			virtual ~OpenGl3Mesh() throw();

			virtual void bind(BitSet32 &used_attributes);
			virtual void unbind();
			virtual AbstractMeshSharedPtr clone_vertex_data();

	};

}

#endif	/* UUID_ed58a002_ed47_43c1_acda_0bd30dbb9d7a */

