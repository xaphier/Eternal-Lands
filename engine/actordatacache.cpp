/****************************************************************************
 *            actordatacache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "actordatacache.hpp"
#include "loader/cal3dloader.hpp"
#include "abstractmesh.hpp"
#include "meshbuilder.hpp"
#include "meshdatatool.hpp"
#include "indexupdatesource.hpp"
#include "material.hpp"
#include "materialbuilder.hpp"
#include "materialcache.hpp"
#include "materialdescription.hpp"
#include "materialdescriptioncache.hpp"
#include "objectdata.hpp"
#include "actor.hpp"
#include "exceptions.hpp"
#include "globalvars.hpp"
#include "filesystem.hpp"

namespace eternal_lands
{

	class ActorDataCache::ActorDataCacheItem
	{
		public:
			AbstractMeshSharedPtr m_mesh;
			IndexUpdateSourceSharedPtr m_index_source;
			CalCoreModel* m_core_model;
			String m_name;
			String m_file_name;
			String m_material_name;
			float m_actor_scale;
			float m_scale;
			float m_mesh_scale;
			float m_skel_scale;
			bool m_ghost;

	};

	ActorDataCache::ActorDataCache(const MeshBuilderWeakPtr &mesh_builder,
		const CodecManagerWeakPtr &codec_manager,
		const MaterialCacheWeakPtr &material_cache,
		const MaterialBuilderWeakPtr &material_builder,
		const MaterialDescriptionCacheWeakPtr
			&material_description_cache,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars):
		m_mesh_builder(mesh_builder), m_codec_manager(codec_manager),
		m_material_cache(material_cache),
		m_material_builder(material_builder),
		m_material_description_cache(material_description_cache),
		m_file_system(file_system), m_global_vars(global_vars)
	{
		assert(!m_mesh_builder.expired());
		assert(!m_codec_manager.expired());
		assert(!m_material_cache.expired());
		assert(!m_material_builder.expired());
		assert(!m_material_description_cache.expired());
		assert(m_file_system.get() != nullptr);
		assert(m_global_vars.get() != nullptr);
	}

	ActorDataCache::~ActorDataCache() noexcept
	{
	}

	void ActorDataCache::add_actor(const Uint32 id,
		CalCoreModel *core_model, const String &name,
		const String &skin_name, const String &file_name,
		const float actor_scale, const float scale,
		const float mesh_scale, const float skel_scale,
		const bool ghost)
	{
		ActorDataCacheItem data;
		Cal3dLoader loader(core_model, name);
		MeshDataToolSharedPtr mesh_data_tool;
		String material;
		bool use_16_bit_indices;

		loader.load(false, get_global_vars()->get_use_simd(),
			mesh_data_tool);

		data.m_mesh = get_mesh_builder()->get_mesh(vft_animated_mesh,
			mesh_data_tool, name);

		use_16_bit_indices = mesh_data_tool->get_vertex_count()
			<= std::numeric_limits<Uint16>::max();

		data.m_index_source = boost::make_shared<IndexUpdateSource>(
			mesh_data_tool->get_indices(),
			mesh_data_tool->get_sub_meshs(),
			use_16_bit_indices, false);

		material = FileSystem::get_file_name_without_extension(
			skin_name);

		data.m_core_model = core_model;
		data.m_name = name;
		data.m_material_name = material;
		data.m_file_name = file_name;
		data.m_actor_scale = actor_scale;
		data.m_scale = scale;
		data.m_mesh_scale = mesh_scale;
		data.m_skel_scale = skel_scale;
		data.m_ghost = ghost;

		m_actor_data_cache[id] = data;
	}

	std::auto_ptr<Actor> ActorDataCache::get_actor(const Uint32 type_id,
		const Uint32 id, const String &name,
		const SelectionType selection, const bool enhanced_actor)
	{
		MaterialSharedPtrVector materials;
		MaterialSharedPtr material;
		MaterialDescription material_description;
		ActorDataCacheMap::const_iterator found;
		std::auto_ptr<Actor> result;
		VertexStreamBitset all;
		BlendType blend;

		found = m_actor_data_cache.find(type_id);

		if (found == m_actor_data_cache.end())
		{
			EL_THROW_EXCEPTION(ItemNotFoundException()
				<< errinfo_item_id(type_id)
				<< errinfo_value(id)
				<< errinfo_string_value(name));
		}

		if (enhanced_actor)
		{
			material_description = get_material_description_cache(
				)->get_material_description(
				found->second.m_material_name);

			material = get_material_builder()->get_material(
				material_description);
		}
		else
		{
			material = get_material_cache()->get_material(
				found->second.m_material_name);
		}

		materials.clear();
		materials.push_back(material);

		if (found->second.m_ghost)
		{
			blend = bt_alpha_transparency_value;
		}
		else
		{
			blend = bt_disabled;
		}

		all.set();

		if (enhanced_actor)
		{
			result.reset(new Actor(ObjectData(Transformation(),
				name, 0.7f, id, selection, blend),
				found->second.m_mesh->clone(all, false),
				materials, found->second.m_index_source,
				found->second.m_core_model));

			result->init_enhanced_actor(get_codec_manager(),
				get_file_system(), get_global_vars());
		}
		else
		{
			result.reset(new Actor(ObjectData(Transformation(),
				name, 0.7f, id, selection, blend),
				found->second.m_mesh, materials,
				found->second.m_core_model));
		}

		return result;
	}

}
