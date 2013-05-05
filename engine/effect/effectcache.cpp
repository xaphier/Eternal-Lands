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
#include "filesystem.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	EffectCache::EffectCache(const GlslProgramCacheWeakPtr
			&glsl_program_cache,
		const ShaderSourceBuilderConstWeakPtr &shader_source_builder):
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

	void EffectCache::load_effect(
		const FileSystemConstSharedPtr &file_system,
		const String &file_name)
	{
		EffectDescription effect_description;
		XmlReaderSharedPtr xml_reader;
		String name;

		try
		{
			if (!XmlReader::get_xml_reader(file_system,
				file_name, xml_reader))
			{
				return;
			}

			name = String(std::string(reinterpret_cast<const char*>(
				xml_reader->get_root_node()->name)));

			if (name != EffectDescription::get_xml_id())
			{
				return;
			}

			effect_description.load_xml(
				xml_reader->get_root_node());

			name = effect_description.get_name();

			m_effect_cache[name] = build_effect(
				effect_description);

			LOG_DEBUG(lt_effect, UTF8("Effect '%1%' loaded from "
				"file '%2%'"), name % file_name);
		}
		catch (boost::exception &exception)
		{
			exception << boost::errinfo_file_name(file_name);

			LOG_EXCEPTION(exception);
		}
	}

	void EffectCache::load_xml(const FileSystemConstSharedPtr &file_system,
		const String &dir)
	{
		StringSet files;

		files = file_system->get_files(dir, String(UTF8("*.xml")));

		BOOST_FOREACH(const String &file, files)
		{
			load_effect(file_system, file);
		}
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

	void EffectCache::add_effect(
		const EffectDescription &effect_description)
	{
		String name;

		name = effect_description.get_name();

		m_effect_cache[name] = build_effect(effect_description);

		LOG_DEBUG(lt_effect, UTF8("Effect '%1%' added"), name);
	}

	EffectSharedPtr EffectCache::build_effect(
		const EffectDescription &effect_description) const
	{
		return boost::make_shared<Effect>(get_glsl_program_cache(),
			get_shader_source_builder(), effect_description);
	}

}
