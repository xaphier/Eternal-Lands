/****************************************************************************
 *            actordatacache.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "actordatacache.hpp"
#include "loader/cal3dloader.hpp"
#include "abstractmesh.hpp"
#include "meshbuilder.hpp"
#include "meshdatatool.hpp"
#include "indexupdatesource.hpp"
#include "materialdescription.hpp"
#include "objectdata.hpp"
#include "actor.hpp"
#include "exceptions.hpp"

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
			String m_skin_name;
			float m_actor_scale;
			float m_scale;
			float m_mesh_scale;
			float m_skel_scale;
			bool m_ghost;

	};

	ActorDataCache::ActorDataCache(const MeshBuilderWeakPtr &mesh_builder,
		const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const CodecManagerWeakPtr &codec_manager,
		const FileSystemWeakPtr &file_system,
		const GlobalVarsSharedPtr &global_vars):
		m_mesh_builder(mesh_builder), m_effect_cache(effect_cache),
		m_texture_cache(texture_cache), m_codec_manager(codec_manager),
		m_file_system(file_system), m_global_vars(global_vars)
	{
		assert(!m_mesh_builder.expired());
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());
		assert(!m_codec_manager.expired());
		assert(!m_file_system.expired());
		assert(m_global_vars.get() != 0);
	}

	ActorDataCache::~ActorDataCache() throw()
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
		Cal3dLoader loader(core_model);
		MeshDataToolSharedPtr mesh_data_tool;
		bool use_16_bit_indices;

		loader.load(mesh_data_tool, false);

		data.m_mesh = get_mesh_builder()->get_mesh(vft_animated_mesh,
			mesh_data_tool, name);

		use_16_bit_indices = mesh_data_tool->get_vertex_count()
			<= std::numeric_limits<Uint16>::max();

		data.m_index_source = boost::make_shared<IndexUpdateSource>(
			mesh_data_tool->get_indices(),
			mesh_data_tool->get_sub_meshs(),
			0, pt_triangles, use_16_bit_indices, false);

		data.m_core_model = core_model;
		data.m_name = name;
		data.m_skin_name = skin_name;
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
		MaterialDescriptionVector materials;
		MaterialDescription material;
		ActorDataCacheMap::const_iterator found;
		std::auto_ptr<Actor> result;
		bool blend;

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
			material.set_effect(String(
				UTF8("animated_mesh_solid")));
		}
		else
		{
			material.set_texture(found->second.m_skin_name,
				stt_diffuse_0);
			material.set_effect(String(
				UTF8("animated_mesh_solid")));
		}

		materials.clear();
		materials.push_back(material);

		if (found->second.m_ghost)
		{
			blend = true;
		}
		else
		{
			blend = false;
		}

		result.reset(new Actor(ObjectData(glm::mat4x3(),
			glm::vec4(0.0f), name, 0.7f, id, selection, blend),
			found->second.m_mesh->clone_vertex_data(), materials,
			get_effect_cache(), get_texture_cache(),
			found->second.m_index_source,
			found->second.m_core_model));

		if (enhanced_actor)
		{
			result->init_enhanced_actor(get_codec_manager(),
				get_file_system(), get_global_vars());
		}

		return result;
	}

}
