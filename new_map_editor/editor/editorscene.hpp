/****************************************************************************
 *            editorscene.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_bd1cf1bf_f1c7_4ef9_914f_843f4df019e8
#define	UUID_bd1cf1bf_f1c7_4ef9_914f_843f4df019e8

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "scene.hpp"

/**
 * @file
 * @brief The @c class EditorScene.
 * This file contains the @c class EditorScene.
 */
namespace eternal_lands
{

	class EditorMapData;

	/**
	 * @brief @c class for scenes used in the editor.
	 *
	 * @c class for scenes used in the editor.
	 */
	class EditorScene: public Scene
	{
		private:
			Uint32ObjectSharedPtrMap m_light_objects;

		protected:
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const;

		public:
			/**
			 * Default constructor.
			 */
			EditorScene(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system);

			/**
			 * Default destructor.
			 */
			virtual ~EditorScene() throw();
			virtual void add_light(const LightData &light_data);
			virtual void remove_light(const Uint32 id);
			void load_map(const String &name, EditorMapData &data);

	};

}

#endif	/* UUID_bd1cf1bf_f1c7_4ef9_914f_843f4df019e8 */
