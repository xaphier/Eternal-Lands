/****************************************************************************
 *            editorscene.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editorscene.hpp"
#include "frustum.hpp"
#include "object.hpp"
#include "materialbuilder.hpp"
#include "materialdescription.hpp"
#include "lightdata.hpp"
#include "meshcache.hpp"
#include "editormapdata.hpp"
#include "editormaploader.hpp"
#include "freeidsmanager.hpp"
#include "map.hpp"

namespace eternal_lands
{

	EditorScene::EditorScene(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system):
		Scene(global_vars, file_system), m_draw_lights(false),
		m_draw_light_spheres(false)
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
		MaterialSharedPtrVector materials;
		MaterialDescription material_description;
		ObjectData object_data;
		Transformation transformation;
		glm::mat2x3 emission_scale_offset;
		Uint32 object_id;

		object_id = get_free_ids()->use_typeless_object_id(
			light_data.get_id(), it_light_object);

		transformation.set_translation(light_data.get_position());
		object_data.set_name(String(UTF8("sphere")));
		object_data.set_selection(st_select);
		object_data.set_world_transformation(transformation);
		object_data.set_id(object_id);
		material_description.set_cast_shadows(false);

		get_scene_resources().get_mesh_cache()->get_mesh(
			object_data.get_name(), mesh);

		emission_scale_offset[1] = light_data.get_color();

		material_description.set_emission_scale_offset(
			emission_scale_offset);
		material_description.set_name(String(UTF8("light")));
		material_description.set_effect(String(UTF8("solid-color")));

		materials.push_back(get_scene_resources().get_material_builder(
			)->get_material(material_description));

		object = boost::make_shared<Object>(object_data, mesh,
			materials);

		temp = m_light_objects.insert(
			Uint32ObjectSharedPtrMap::value_type(
			object_data.get_id(), object));

		assert(temp.second);

		materials.clear();
		materials.push_back(get_scene_resources().get_material_builder(
			)->get_material(material_description));

		transformation.set_scale(light_data.get_radius());
		object_data.set_world_transformation(transformation);
		object_data.set_selection(st_none);

		object = boost::make_shared<Object>(object_data, mesh,
			materials);

		temp = m_light_sphere_objects.insert(
			Uint32ObjectSharedPtrMap::value_type(
			object_data.get_id(), object));

		assert(temp.second);

		Scene::add_light(light_data);
	}

	void EditorScene::remove_light(const Uint32 id)
	{
		Uint32ObjectSharedPtrMap::iterator found;
		Uint32 object_id;

		object_id = get_free_ids()->get_object_id(id, it_light_object);

		found = m_light_objects.find(object_id);

		if (found != m_light_objects.end())
		{
			m_light_objects.erase(found);
		}

		found = m_light_sphere_objects.find(object_id);

		if (found != m_light_sphere_objects.end())
		{
			m_light_sphere_objects.erase(found);
		}

		get_free_ids()->free_object_id(object_id);

		Scene::remove_light(id);
	}

	void EditorScene::intersect(const Frustum &frustum,
		ObjectVisitor &visitor) const
	{
		Uint32ObjectSharedPtrMap::const_iterator it, begin, end;
		float transparency;
		SubFrustumsMask mask;
		BlendType blend;

		Scene::intersect(frustum, visitor);

		if (!get_draw_lights())
		{
			return;
		}

		if (get_draw_light_spheres())
		{
			end = m_light_sphere_objects.end();
			begin = m_light_sphere_objects.begin();
			blend = bt_alpha_transparency_value;
			transparency = 0.5f;
		}
		else
		{
			end = m_light_objects.end();
			begin = m_light_objects.begin();
			blend = bt_disabled;
			transparency = 1.0f;
		}

		for (it = begin; it != end; ++it)
		{
			mask = frustum.intersect_sub_frustums(
				it->second->get_bounding_box());

			if (mask.any())
			{
				visitor.add(it->second, transparency, blend,
					mask);
			}
		}
	}

	void EditorScene::load_map(const String &name, EditorMapData &data)
	{
		boost::scoped_ptr<EditorMapLoader> map_loader;

		clear();
		m_light_objects.clear();
		m_light_sphere_objects.clear();

		map_loader.reset(new EditorMapLoader(
			get_scene_resources().get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_scene_resources().get_mesh_builder(),
			get_scene_resources().get_mesh_cache(),
			get_scene_resources().get_mesh_data_cache(),
			get_scene_resources().get_material_cache(),
			get_free_ids(), data));

		set_map(map_loader->get_map(name));

		map_loader->load(name);
	}

}