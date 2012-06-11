/****************************************************************************
 *            cdlodterrainmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
		const ImageSharedPtr &vector_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const GlobalVarsSharedPtr &global_vars,
		const MeshCacheSharedPtr &mesh_cache,
		const MaterialSharedPtr &material): m_material(material)
	{
		TextureSharedPtr vector_texture;
		TextureSharedPtr normal_texture;
		TextureSharedPtr dudv_texture;
		StringStream str;
		glm::vec3 min, max;

		set_terrain_size((glm::vec2(vector_map->get_sizes()) -1.0f) *
			get_patch_scale());

		str << UTF8("terrain_") << CdLodQuadTree::get_patch_size();

		mesh_cache->get_mesh(String(str.str()), m_mesh);

		m_cd_lod_quad_tree.reset(new CdLodQuadTree(vector_map,
			get_vector_scale(), get_patch_scale()));

		max.x += m_cd_lod_quad_tree->get_grid_size().x *
			get_patch_scale();
		max.y += m_cd_lod_quad_tree->get_grid_size().y *
			get_patch_scale();

		set_bounding_box(BoundingBox(min, max));

		vector_texture = boost::make_shared<Texture>(
			vector_map->get_name());

		vector_texture->set_mipmap_count(0);
		vector_texture->set_wrap_s(twt_clamp);
		vector_texture->set_wrap_t(twt_clamp);
		vector_texture->set_wrap_r(twt_clamp);
		vector_texture->set_target(ttt_texture_rectangle);
		vector_texture->set_format(vector_map->get_texture_format());
		vector_texture->set_image(vector_map);

		m_material->set_texture(vector_texture, spt_vertex_vector);

		normal_texture = boost::make_shared<Texture>(
			normal_map->get_name());

		normal_texture->set_mipmap_count(0);
		normal_texture->set_wrap_s(twt_clamp);
		normal_texture->set_wrap_t(twt_clamp);
		normal_texture->set_wrap_r(twt_clamp);
		normal_texture->set_target(ttt_texture_rectangle);
		normal_texture->set_format(normal_map->get_texture_format());
		normal_texture->set_image(normal_map);

		m_material->set_texture(normal_texture, spt_vertex_normal);

		dudv_texture = boost::make_shared<Texture>(
			dudv_map->get_name());

		dudv_texture->set_mipmap_count(0);
		dudv_texture->set_wrap_s(twt_clamp);
		dudv_texture->set_wrap_t(twt_clamp);
		dudv_texture->set_wrap_r(twt_clamp);
		dudv_texture->set_target(ttt_texture_rectangle);
		dudv_texture->set_format(dudv_map->get_texture_format());
		dudv_texture->set_image(dudv_map);

		m_material->set_texture(dudv_texture, spt_vertex_dudv);
	}

	CdLodTerrainManager::~CdLodTerrainManager() noexcept
	{
	}

	void CdLodTerrainManager::set_clipmap_texture(
		const TextureSharedPtr &texture)
	{
		m_material->set_texture(texture, spt_albedo_0);
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

	void CdLodTerrainManager::update(const ImageSharedPtr &vector_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map)
	{
		TextureSharedPtr vector_texture;
		TextureSharedPtr normal_texture;
		TextureSharedPtr dudv_texture;
		glm::vec3 min, max;

		set_terrain_size((glm::vec2(vector_map->get_sizes()) -1.0f) *
			get_patch_scale());

		m_cd_lod_quad_tree.reset(new CdLodQuadTree(vector_map,
			get_vector_scale(), get_patch_scale()));

		max.x += m_cd_lod_quad_tree->get_grid_size().x *
			get_patch_scale();
		max.y += m_cd_lod_quad_tree->get_grid_size().y *
			get_patch_scale();

		set_bounding_box(BoundingBox(min, max));

		vector_texture = boost::make_shared<Texture>(
			vector_map->get_name());

		vector_texture->set_mipmap_count(0);
		vector_texture->set_wrap_s(twt_clamp);
		vector_texture->set_wrap_t(twt_clamp);
		vector_texture->set_wrap_r(twt_clamp);
		vector_texture->set_target(ttt_texture_rectangle);
		vector_texture->set_format(vector_map->get_texture_format());
		vector_texture->set_image(vector_map);

		m_material->set_texture(vector_texture, spt_vertex_vector);

		normal_texture = boost::make_shared<Texture>(
			normal_map->get_name());

		normal_texture->set_mipmap_count(0);
		normal_texture->set_wrap_s(twt_clamp);
		normal_texture->set_wrap_t(twt_clamp);
		normal_texture->set_wrap_r(twt_clamp);
		normal_texture->set_target(ttt_texture_rectangle);
		normal_texture->set_format(normal_map->get_texture_format());
		normal_texture->set_image(normal_map);

		m_material->set_texture(normal_texture, spt_vertex_normal);

		dudv_texture = boost::make_shared<Texture>(
			dudv_map->get_name());

		dudv_texture->set_mipmap_count(0);
		dudv_texture->set_wrap_s(twt_clamp);
		dudv_texture->set_wrap_t(twt_clamp);
		dudv_texture->set_wrap_r(twt_clamp);
		dudv_texture->set_target(ttt_texture_rectangle);
		dudv_texture->set_format(dudv_map->get_texture_format());
		dudv_texture->set_image(dudv_map);

		m_material->set_texture(dudv_texture, spt_vertex_dudv);
	}

}
