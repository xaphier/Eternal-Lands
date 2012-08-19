/****************************************************************************
 *            effectcache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectcache.hpp"
#include "effect.hpp"
#include "effectdescription.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	EffectCache::EffectCache(const GlslProgramCacheWeakPtr
			&glsl_program_cache, const ShaderSourceBuilderWeakPtr
			&shader_source_builder):
		m_glsl_program_cache(glsl_program_cache),
		m_shader_source_builder(shader_source_builder)
	{
		assert(!m_glsl_program_cache.expired());
		assert(!m_shader_source_builder.expired());
	}

	EffectCache::~EffectCache() noexcept
	{
	}

	const EffectSharedPtr &EffectCache::get_effect(const String &name)
	{
		EffectCacheMap::iterator found;

		found = m_effect_cache.find(name);

		if (found == m_effect_cache.end())
		{
			LOG_ERROR(lt_effect, UTF8("Effect '%1%' not found"),
				name);

			return get_simple_effect();
		}
		else
		{
			assert(found->second.get() != nullptr);

			return found->second;
		}
	}

	const EffectSharedPtr &EffectCache::get_simple_effect()
	{
		if (m_simple_effect.get() == nullptr)
		{
			m_simple_effect = boost::make_shared<Effect>(
				get_glsl_program_cache());
		}

		return m_simple_effect;
	}

	void EffectCache::reload()
	{
		EffectCacheMap::iterator it, end;

		end = m_effect_cache.end();

		for (it = m_effect_cache.begin(); it != end; ++it)
		{
			it->second->load();
		}
	}

	void EffectCache::set_debug_shader(const ShaderBuildType debug)
	{
		EffectCacheMap::iterator it, end;

		end = m_effect_cache.end();

		for (it = m_effect_cache.begin(); it != end; ++it)
		{
			it->second->set_debug_shader(debug);
		}
	}

	void EffectCache::load_xml(const xmlNodePtr node)
	{
		String name;
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("effects")) != 0)
		{
			return;
		}

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("effect"))
				== 0)
			{
				EffectDescription effect_description;

				effect_description.load_xml(it);

				name = effect_description.get_name();

				m_effect_cache[name] =
					boost::make_shared<Effect>(
						get_glsl_program_cache(),
						get_shader_source_builder(),
						effect_description);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void EffectCache::load_xml(const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(xml_reader->get_root_node());
	}

	StringVector EffectCache::get_effect_names() const
	{
		EffectCacheMap::const_iterator it, end;
		StringVector result;

		end = m_effect_cache.end();

		for (it = m_effect_cache.begin(); it != end; ++it)
		{
			result.push_back(it->first);
		}

		return result;
	}

	void EffectCache::save_xml(const String &file_name) const
	{
		EffectCacheMap::const_iterator it, end;
		XmlWriterSharedPtr writer;

		writer = XmlWriterSharedPtr(new XmlWriter(file_name));

		writer->start_element(UTF8("effects"));

		end = m_effect_cache.end();

		for (it = m_effect_cache.begin(); it != end; ++it)
		{
			it->second->get_description().save_xml(writer);
		}

		writer->end_element();
	}

}
