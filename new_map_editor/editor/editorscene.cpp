/****************************************************************************
 *            editorscene.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editorscene.hpp"
#include "frustum.hpp"
#include "object.hpp"
#include "materialdescription.hpp"
#include "lightdata.hpp"
#include "meshcache.hpp"

namespace eternal_lands
{

	EditorScene::EditorScene(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system):
		Scene(global_vars, file_system)
	{
	}

	EditorScene::~EditorScene() throw()
	{
	}

	void EditorScene::add_light(const LightData &light_data)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;
		MaterialDescriptionVector materials;
		ObjectData object_data;
		Transformation transformation;

		transformation.set_translation(light_data.get_position());
		object_data.set_name(String(UTF8("sphere")));
		object_data.set_selection(st_select);
		object_data.set_world_transformation(transformation);

		get_scene_resources().get_mesh_cache()->get_mesh(
			object_data.get_name(), mesh, materials);

		object = boost::make_shared<Object>(object_data, mesh,
			materials, get_scene_resources().get_effect_cache(),
			get_scene_resources().get_texture_cache());

		temp = m_light_objects.insert(
			Uint32ObjectSharedPtrMap::value_type(
			object_data.get_id(), object));

		assert(temp.second);

		Scene::add_light(light_data);
	}

	void EditorScene::remove_light(const Uint32 id)
	{
		Uint32ObjectSharedPtrMap::iterator found;

		found = m_light_objects.find(id);

		if (found != m_light_objects.end())
		{
			m_light_objects.erase(found);
		}

		Scene::remove_light(id);
	}

	void EditorScene::intersect(const Frustum &frustum,
		ObjectVisitor &visitor) const
	{
		Uint32ObjectSharedPtrMap::const_iterator it, end;
		SubFrustumsMask mask;
		BlendType blend;

		end = m_light_objects.end();
		blend = bt_alpha_transparency_value;

		for (it = m_light_objects.begin(); it != end; ++it)
		{
			mask = frustum.intersect_sub_frustums(
				it->second->get_bounding_box());

			if (mask.any())
			{
				visitor.add(it->second, 0.5f, blend, mask);
			}
		}
	}

}
