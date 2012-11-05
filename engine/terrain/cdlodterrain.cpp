/****************************************************************************
 *            cdlodterrain.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cdlodterrain.hpp"
#include "material.hpp"
#include "terrainvisitor.hpp"
#include "abstractmesh.hpp"
#include "meshcache.hpp"
#include "cdlodquadtree.hpp"
#include "texture.hpp"
#include "image.hpp"
#include "imageupdate.hpp"
#include "globalvars.hpp"

namespace eternal_lands
{

	CdLodTerrain::CdLodTerrain(const GlobalVarsSharedPtr &global_vars,
		const EffectCacheSharedPtr &effect_cache,
		const MeshCacheSharedPtr &mesh_cache,
		const MaterialBuilderSharedPtr &material_builder,
		const MaterialCacheSharedPtr &material_cache,
		const String &material, const String &effect):
		AbstractTerrain(global_vars, effect_cache, material_builder,
			material_cache, material, effect)
	{
		StringStream str;

		str << UTF8("terrain_") << CdLodQuadTree::get_patch_size();

		mesh_cache->get_mesh(String(str.str()), m_mesh);

		m_cd_lod_quad_tree.reset(new CdLodQuadTree());
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
			bounding_box);
	}

	void CdLodTerrain::intersect(const Frustum &frustum,
		const glm::vec3 &camera, TerrainVisitor &terrain) const
	{
		BoundingBox bounding_box;
		Uint32 instance_count;

		terrain.set_instances(0);

		terrain.set_mesh(m_mesh);
		terrain.set_material(get_terrain_material());

		m_cd_lod_quad_tree->select_quads_for_drawing(frustum, camera,
			terrain.get_mapped_uniform_buffer(), bounding_box,
			instance_count);

		terrain.set_instances(instance_count);

		terrain.set_bounding_box(bounding_box);
	}

	void CdLodTerrain::do_set_geometry_maps(
		const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map)
	{
		glm::vec3 min, max;
		float patch_scale;

		set_terrain_size((glm::vec2(displacement_map->get_size())
			-1.0f) * get_patch_scale());

		patch_scale = get_patch_scale();

		m_cd_lod_quad_tree->init(displacement_map->decompress(false,
			true, false), patch_scale);

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

		get_terrain_material()->set_texture(m_displacement_texture,
			spt_effect_8);

		m_normal_texture = boost::make_shared<Texture>(
			normal_map->get_name(), normal_map->get_width(),
			normal_map->get_height(), 1, 0xFFFF, 0,
			normal_map->get_texture_format(),
			ttt_texture_2d);

		m_normal_texture->set_wrap_s(twt_clamp);
		m_normal_texture->set_wrap_t(twt_clamp);
		m_normal_texture->set_wrap_r(twt_clamp);
		m_normal_texture->set_image(normal_map);

		get_terrain_material()->set_texture(m_normal_texture,
			spt_effect_9);

		m_dudv_texture = boost::make_shared<Texture>(
			dudv_map->get_name(), dudv_map->get_width(),
			dudv_map->get_height(), 1, 0xFFFF, 0,
			dudv_map->get_texture_format(), ttt_texture_2d);

		m_dudv_texture->set_wrap_s(twt_clamp);
		m_dudv_texture->set_wrap_t(twt_clamp);
		m_dudv_texture->set_wrap_r(twt_clamp);
		m_dudv_texture->set_image(dudv_map);

		get_terrain_material()->set_texture(m_dudv_texture,
			spt_effect_10);
	}

	void CdLodTerrain::do_update_geometry_maps(
		const ImageUpdate &displacement_map,
		const ImageUpdate &normal_map, const ImageUpdate &dudv_map)
	{
		glm::vec3 min, max;
		float patch_scale;

		set_terrain_size((glm::vec2(displacement_map.get_image(
			)->get_size()) -1.0f) * get_patch_scale());

		patch_scale = get_patch_scale();

		m_cd_lod_quad_tree->init(displacement_map.get_image(
			)->decompress(false, true, false), patch_scale);

		min = m_cd_lod_quad_tree->get_min();
		max = m_cd_lod_quad_tree->get_max();

		max.x += m_cd_lod_quad_tree->get_grid_size().x *
			get_patch_scale();
		max.y += m_cd_lod_quad_tree->get_grid_size().y *
			get_patch_scale();

		set_bounding_box(BoundingBox(min, max));

		m_displacement_texture->sub_texture(displacement_map);
		m_normal_texture->sub_texture(normal_map);
		m_dudv_texture->sub_texture(dudv_map);
	}

	void CdLodTerrain::clear()
	{
		m_cd_lod_quad_tree->clear();
		set_bounding_box(BoundingBox());
	}

}
