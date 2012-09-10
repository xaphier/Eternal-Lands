/****************************************************************************
 *            cdlodterrainmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cdlodterrainmanager.hpp"
#include "material.hpp"
#include "terrainvisitor.hpp"
#include "abstractmesh.hpp"
#include "meshcache.hpp"
#include "cdlodquadtree.hpp"
#include "texture.hpp"
#include "image.hpp"

namespace eternal_lands
{

	CdLodTerrainManager::CdLodTerrainManager(
		const MeshCacheSharedPtr &mesh_cache,
		const MaterialSharedPtr &material): m_material(material)
	{
		StringStream str;

		str << UTF8("terrain_") << CdLodQuadTree::get_patch_size();

		mesh_cache->get_mesh(String(str.str()), m_mesh);

		m_cd_lod_quad_tree.reset(new CdLodQuadTree());
	}

	CdLodTerrainManager::~CdLodTerrainManager() noexcept
	{
	}

	void CdLodTerrainManager::set_clipmap_texture(
		const TextureSharedPtr &texture)
	{
		m_material->set_texture(texture, spt_effect_0);
	}

	void CdLodTerrainManager::intersect(const Frustum &frustum,
		ObjectVisitor &visitor) const
	{
	}

	void CdLodTerrainManager::intersect(const Frustum &frustum,
		const glm::vec3 &camera, BoundingBox &bounding_box) const
	{
		m_cd_lod_quad_tree->select_bounding_box(frustum, camera,
			bounding_box);
	}

	void CdLodTerrainManager::intersect(const Frustum &frustum,
		const glm::vec3 &camera, TerrainVisitor &terrain) const
	{
		BoundingBox bounding_box;
		Uint32 instance_count;

		terrain.set_instances(0);

		terrain.set_mesh(m_mesh);
		terrain.set_material(m_material);

		m_cd_lod_quad_tree->select_quads_for_drawing(frustum, camera,
			terrain.get_mapped_uniform_buffer(), bounding_box,
			instance_count);

		terrain.set_instances(instance_count);

		terrain.set_bounding_box(bounding_box);
	}

	void CdLodTerrainManager::update(const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map)
	{
		glm::vec3 min, max;

		set_terrain_size((glm::vec2(displacement_map->get_sizes())
			-1.0f) * get_patch_scale());

		m_cd_lod_quad_tree->init(displacement_map->decompress(false,
			true), get_patch_scale());

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

		m_material->set_texture(m_displacement_texture, spt_effect_8);

		m_normal_texture = boost::make_shared<Texture>(
			normal_map->get_name(), normal_map->get_width(),
			normal_map->get_height(), 1, 0xFFFF, 0,
			normal_map->get_texture_format(),
			ttt_texture_2d);

		m_normal_texture->set_wrap_s(twt_clamp);
		m_normal_texture->set_wrap_t(twt_clamp);
		m_normal_texture->set_wrap_r(twt_clamp);
		m_normal_texture->set_image(normal_map);

		m_material->set_texture(m_normal_texture, spt_effect_9);

		m_dudv_texture = boost::make_shared<Texture>(
			dudv_map->get_name(), dudv_map->get_width(),
			dudv_map->get_height(), 1, 0xFFFF, 0,
			dudv_map->get_texture_format(), ttt_texture_2d);

		m_dudv_texture->set_wrap_s(twt_clamp);
		m_dudv_texture->set_wrap_t(twt_clamp);
		m_dudv_texture->set_wrap_r(twt_clamp);
		m_dudv_texture->set_image(dudv_map);

		m_material->set_texture(m_dudv_texture, spt_effect_10);
	}

	void CdLodTerrainManager::clear()
	{
		m_cd_lod_quad_tree->clear();
		set_bounding_box(BoundingBox());
	}

	TextureSharedPtr CdLodTerrainManager::get_displacement_texture() const
	{
		return m_displacement_texture;
	}

	TextureSharedPtr CdLodTerrainManager::get_normal_texture() const
	{
		return m_normal_texture;
	}

	TextureSharedPtr CdLodTerrainManager::get_dudv_texture() const
	{
		return m_dudv_texture;
	}

}
