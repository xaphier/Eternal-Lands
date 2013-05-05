/****************************************************************************
 *            cdlodterrain.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cdlodterrain.hpp"
#include "material.hpp"
#include "terrainvisitor.hpp"
#include "terrainrenderingdata.hpp"
#include "abstractmesh.hpp"
#include "meshbuilder.hpp"
#include "meshcache.hpp"
#include "meshdatacache.hpp"
#include "cdlodquadtree.hpp"
#include "texture.hpp"
#include "image.hpp"
#include "globalvars.hpp"

namespace eternal_lands
{

	CdLodTerrain::CdLodTerrain(const GlobalVarsConstSharedPtr &global_vars,
		const EffectCacheSharedPtr &effect_cache,
		const MeshBuilderConstSharedPtr &mesh_builder,
		const MeshCacheSharedPtr &mesh_cache,
		const MeshDataCacheSharedPtr &mesh_data_cache,
		const MaterialBuilderConstSharedPtr &material_builder,
		const MaterialCacheSharedPtr &material_cache,
		const StringArray3 &material, const String &effect):
		AbstractTerrain(global_vars, effect_cache, material_builder,
			material_cache, material, effect)
	{
		StringStream str;
		String mesh_name, name;

		str << UTF8("terrain_") << CdLodQuadTree::get_patch_size();

		mesh_name = String(str.str());

		str << UTF8("-instanced-") << 1024;

		name = String(str.str());

		if (!mesh_cache->get_has_mesh(name))
		{
			MeshDataToolSharedPtr mesh_data_tool;

			mesh_data_cache->get_mesh_data(mesh_name,
				mesh_data_tool);

			m_mesh = mesh_builder->get_mesh(vft_cdlod_terrain,
				mesh_data_tool, name, 1024, true, true, false);
		}
		else
		{
			mesh_cache->get_mesh(name, m_mesh);
		}

		m_cd_lod_quad_tree.reset(new CdLodQuadTree(
			get_low_quality_terrain()));
	}

	CdLodTerrain::~CdLodTerrain() noexcept
	{
	}

	void CdLodTerrain::intersect(const Frustum &frustum,
		ObjectVisitor &visitor) const
	{
	}

	void CdLodTerrain::intersect(const Frustum &frustum,
		const glm::vec3 &camera, BoundingBox &bounding_box) const
	{
		m_cd_lod_quad_tree->select_bounding_box(frustum, camera,
			0xFFFF, bounding_box);
	}

	void CdLodTerrain::intersect(const Frustum &frustum,
		const glm::vec3 &camera, TerrainVisitor &terrain) const
	{
		BoundingBox bounding_box;
		glm::vec4 terrain_lod_offset;
		Uint32 instances;

		terrain.set_instances(0);

		terrain.set_material(get_material());

		m_cd_lod_quad_tree->select_quads_for_drawing(frustum, camera,
			terrain.get_buffer(), terrain.get_offset(),
			terrain.get_max_instances(), bounding_box,
			terrain_lod_offset, instances);

		terrain.set_instances(instances);
		terrain.set_terrain_lod_offset(terrain_lod_offset);
		terrain.set_bounding_box(bounding_box);
	}

	void CdLodTerrain::init_rendering_data(
		TerrainRenderingData &rendering_data) const
	{
		rendering_data.set_mesh(m_mesh->clone(0x01, true));
		rendering_data.set_offset(0);
		rendering_data.set_max_instances(m_mesh->get_instance_count());
	}

	void CdLodTerrain::do_set_geometry_maps(
		const ImageConstSharedPtr &displacement_map,
		const ImageConstSharedPtr &normal_tangent_map,
		const ImageConstSharedPtr &dudv_map)
	{
		glm::vec3 min, max;
		float patch_scale;

		set_size((glm::vec2(displacement_map->get_size()) - 1.0f) *
			get_patch_scale());

		patch_scale = get_patch_scale();

		m_cd_lod_quad_tree->init(
			displacement_map->decompress(false, true, false),
			get_translation(), patch_scale);

		min = m_cd_lod_quad_tree->get_min();
		max = m_cd_lod_quad_tree->get_max();

		max.x += m_cd_lod_quad_tree->get_grid_size().x *
			get_patch_scale();
		max.y += m_cd_lod_quad_tree->get_grid_size().y *
			get_patch_scale();

		set_bounding_box(BoundingBox(min, max));

		m_displacement_texture = boost::make_shared<Texture>(
			displacement_map->get_name(),
			displacement_map->get_width(),
			displacement_map->get_height(), 1, 0xFFFF, 0,
			displacement_map->get_texture_format(),
			ttt_texture_2d);

		m_displacement_texture->set_wrap_s(twt_clamp);
		m_displacement_texture->set_wrap_t(twt_clamp);
		m_displacement_texture->set_wrap_r(twt_clamp);
		m_displacement_texture->set_image(displacement_map);

		get_material(qt_low)->set_texture(m_displacement_texture,
			spt_effect_8);
		get_material(qt_medium)->set_texture(m_displacement_texture,
			spt_effect_8);
		get_material(qt_high)->set_texture(m_displacement_texture,
			spt_effect_8);

		m_normal_texture = boost::make_shared<Texture>(
			normal_tangent_map->get_name(),
			normal_tangent_map->get_width(),
			normal_tangent_map->get_height(), 1, 0xFFFF, 0,
			normal_tangent_map->get_texture_format(),
			ttt_texture_2d);

		m_normal_texture->set_wrap_s(twt_clamp);
		m_normal_texture->set_wrap_t(twt_clamp);
		m_normal_texture->set_wrap_r(twt_clamp);
		m_normal_texture->set_image(normal_tangent_map);

		get_material(qt_low)->set_texture(m_normal_texture,
			spt_effect_9);
		get_material(qt_medium)->set_texture(m_normal_texture,
			spt_effect_9);
		get_material(qt_high)->set_texture(m_normal_texture,
			spt_effect_9);

		m_dudv_texture = boost::make_shared<Texture>(
			dudv_map->get_name(), dudv_map->get_width(),
			dudv_map->get_height(), 1, 0xFFFF, 0,
			dudv_map->get_texture_format(), ttt_texture_2d);

		m_dudv_texture->set_wrap_s(twt_clamp);
		m_dudv_texture->set_wrap_t(twt_clamp);
		m_dudv_texture->set_wrap_r(twt_clamp);
		m_dudv_texture->set_image(dudv_map);

		get_material(qt_low)->set_texture(m_dudv_texture,
			spt_effect_10);
		get_material(qt_medium)->set_texture(m_dudv_texture,
			spt_effect_10);
		get_material(qt_high)->set_texture(m_dudv_texture,
			spt_effect_10);
	}

	void CdLodTerrain::do_update_geometry_maps(
		const ImageConstSharedPtr &displacement_map,
		const ImageConstSharedPtr &normal_tangent_map,
		const ImageConstSharedPtr &dudv_map)
	{
		glm::vec3 min, max;
		float patch_scale;

		set_size((glm::vec2(displacement_map->get_size()) - 1.0f) *
			get_patch_scale());

		patch_scale = get_patch_scale();

		m_cd_lod_quad_tree->init(
			displacement_map->decompress(false, true, false),
			get_translation(), patch_scale);

		min = m_cd_lod_quad_tree->get_min();
		max = m_cd_lod_quad_tree->get_max();

		max.x += m_cd_lod_quad_tree->get_grid_size().x *
			get_patch_scale();
		max.y += m_cd_lod_quad_tree->get_grid_size().y *
			get_patch_scale();

		set_bounding_box(BoundingBox(min, max));

		m_displacement_texture->update_image(displacement_map);
		m_normal_texture->update_image(normal_tangent_map);
		m_dudv_texture->update_image(dudv_map);
	}

	void CdLodTerrain::clear()
	{
		m_cd_lod_quad_tree->clear();
		set_bounding_box(BoundingBox());
	}

}
