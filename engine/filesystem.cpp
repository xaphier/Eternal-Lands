/****************************************************************************
 *            filesystem.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "filesystem.hpp"
#include "exceptions.hpp"
#include "abstractarchive.hpp"
#include "memorybuffer.hpp"
#include "reader.hpp"
#include "zipfile.hpp"
#include "decompressutil.hpp"
#include <sys/stat.h>

#include "../io/elpathwrapper.h"
#include "../md5.h"
#include "logging.hpp"

namespace eternal_lands
{

	namespace
	{

		class DirArchive: public AbstractArchive
		{
			public:
				/**
				 * Default constructor.
				 */
				DirArchive(const String &name);

				/**
				 * Default destructor.
				 */
				virtual ~DirArchive() throw();

				/**
				 * Opens the file for reading.
				 * @return The Reader object of the file.
				 */
				virtual ReaderSharedPtr get_file(
					const String &file_name) const;

				/**
				 * Checks if the file can be found in the
				 * archive.
				 * @return True if the file is in the archive,
				 * else false
				 */
				virtual bool get_has_file(
					const String &file_name) const;

		};

		DirArchive::DirArchive(const String &name):
			AbstractArchive(name)
		{
		}

		DirArchive::~DirArchive() throw()
		{
		}

		ReaderSharedPtr DirArchive::get_file(const String &file_name)
			const
		{
			ReaderSharedPtr reader;
			MemoryBufferSharedPtr buffer;
			Uint64 size;
			std::ifstream stream;
			StringType path, gz_path, xz_path;
			struct stat fstat;

			path = get_name().get();
			path += '/';
			path += file_name.get();
			path = utf8_to_string(path);

			gz_path = path;
			gz_path += ".gz";

			xz_path = path;
			xz_path += ".xz";

			if (stat(gz_path.c_str(), &fstat) == 0)
			{
				path = gz_path;
			}

			if (stat(xz_path.c_str(), &fstat) == 0)
			{
				path = xz_path;
			}

			if (stat(path.c_str(), &fstat) != 0)
			{
				EL_THROW_EXCEPTION(FileNotFoundException()
					<< boost::errinfo_file_name(file_name));
			}

			size = fstat.st_size;

			stream.open(path.c_str(), std::ifstream::binary);

			buffer = boost::make_shared<MemoryBuffer>(size);

			stream.read(static_cast<char*>(buffer->get_ptr()),
				buffer->get_size());

			reader = boost::make_shared<Reader>(
				DecompresszUtil::decompress(buffer), file_name);

			return reader;
		}

		bool DirArchive::get_has_file(const String &file_name) const
		{
			StringType path, gz_path, xz_path;
			struct stat fstat;

			path = get_name().get();
			path += '/';
			path += file_name.get();
			path = utf8_to_string(path);

			gz_path = path;
			gz_path += ".gz";

			xz_path = path;
			xz_path += ".xz";

			if (stat(gz_path.c_str(), &fstat) == 0)
			{
				return true;
			}

			if (stat(xz_path.c_str(), &fstat) == 0)
			{
				return true;
			}

			return stat(path.c_str(), &fstat) == 0;
		}

		Uint8Array16 get_file_md5(
			const AbstractReadMemoryBufferSharedPtr &buffer)
		{
			MD5 md5;
			Uint8Array16 digest;

			MD5Open(&md5);
			MD5Digest(&md5, buffer->get_ptr(), buffer->get_size());
			MD5Close(&md5, digest.data());

			return digest;
		}

		Uint8Array16 get_file_md5(const String &file_name)
		{
			MemoryBufferSharedPtr buffer;
			Uint64 size;
			std::ifstream stream;
			StringType path, gz_path, xz_path;
			struct stat fstat;

			path = utf8_to_string(file_name);

			if (stat(path.c_str(), &fstat) != 0)
			{
				EL_THROW_EXCEPTION(FileNotFoundException()
					<< boost::errinfo_file_name(file_name));
			}

			size = fstat.st_size;

			stream.open(path.c_str(), std::ifstream::binary);

			buffer = boost::make_shared<MemoryBuffer>(size);

			stream.read(static_cast<char*>(buffer->get_ptr()),
				buffer->get_size());

			return get_file_md5(buffer);
		}

		StringType get_md5_str(const Uint8Array16 &md5)
		{
			StringStream str;

			str << std::hex;

			BOOST_FOREACH(const Uint16 value, md5)
			{
				str << (value >> 4) << (value & 0xF);
			}

			return str.str();
		}

		Uint8Array16 get_md5(const char md5[32])
		{
			Uint8Array16 result;
			Uint16 i, value;

			for (i = 0; i < 16; ++i)
			{
				StringStream str;

				str << md5[i * 2 + 0];
				str << md5[i * 2 + 1];

				str >> std::hex >> value;

				result[i] = value;
			}

			return result;
		}

	}

	FileSystem::FileSystem()
	{
		add_dir(String(UTF8(".")));
/*
		m_md5s[String(UTF8("shaders/shaders.lua"))] =
			get_md5("ff996ce83d55855197b48ce3c29be2ef");
		m_md5s[String(UTF8("shaders/animated_mesh_transparent.xml"))] =
			get_md5("8484847386ddd6ac4e6a9548f083bc9d");
		m_md5s[String(UTF8("shaders/mesh_transparent.xml"))] =
			get_md5("bf6906955a0ae2843e10f6b79b473c51");
		m_md5s[String(UTF8("shaders/lava.xml"))] =
			get_md5("6f8cd1b2eee05e26d661c6ba82313146");
		m_md5s[String(UTF8("shaders/mesh_solid.xml"))] =
			get_md5("b534ba9c8026392b16700dbf8c7131e5");
		m_md5s[String(UTF8("shaders/animated_mesh_solid.xml"))] =
			get_md5("e71912c4118a1c9e3980147a6ac7935f");
		m_md5s[String(UTF8("shaders/sources/diffuse_light.xml"))] =
			get_md5("1ba870316b7f5d88ef47e2a52d7ea808");
		m_md5s[String(UTF8("shaders/sources/layer_index.xml"))] =
			get_md5("6aecdf0b38273d720344a00540bb2e09");
		m_md5s[String(
			UTF8("shaders/sources/no_normal_depth_mapping.xml"))] =
				get_md5("268e62f7d5f9583c0e2c7bc86dc73699");
		m_md5s[String(
			UTF8("shaders/sources/no_specular_mapping.xml"))] =
				get_md5("ff3e88bab4aff36cf607777e2ca8ba19");
		m_md5s[String(UTF8("shaders/sources/lava_uv.xml"))] =
			get_md5("9461805bc1e7441e7be16271aa0a5b16");
		m_md5s[String(UTF8("shaders/sources/view_transform.xml"))] =
			get_md5("a5c6cdc663fdb97639cf3830ec4db304");
		m_md5s[String(UTF8("shaders/sources/specular_light.xml"))] =
			get_md5("0c77090ea6b497979389a7db332f99a7");
		m_md5s[String(
			UTF8("shaders/sources/diffuse_blend_mapping.xml"))] =
				get_md5("fde06ad24d7c8f33236a754d9dedf494");
		m_md5s[String(
			UTF8("shaders/sources/shadow_filtering_mapping.xml"))] =
				get_md5("9c01596c489464186961215d2d7da67d");
		m_md5s[String(
			UTF8("shaders/sources/normal_transform.xml"))] =
				get_md5("bb6b33e3604b7d664fb0e330cfaac82e");
		m_md5s[String(UTF8("shaders/sources/exp_shadow_map.xml"))] =
			get_md5("faedd619a5292c3f95695d15ecda14f5");
		m_md5s[String(
			UTF8("shaders/sources/bone_tangent_animation.xml"))] =
				get_md5("753cfd95e2545ae0591c821e2d489802");
		m_md5s[String(UTF8("shaders/sources/tangent_transform.xml"))] =
			get_md5("c27a2acade4ddf84bb876a7e69d0b1e9");
		m_md5s[String(UTF8("shaders/sources/view_direction.xml"))] =
			get_md5("938c172d9e5fd066e4f6d2e3f517b3ff");
		m_md5s[String(UTF8("shaders/sources/shadow_uv.xml"))] =
			get_md5("957d16a43d63304a86b36ce07281b9e3");
		m_md5s[String(UTF8("shaders/sources/transparent.xml"))] =
			get_md5("54451224df995f28cf31ce9542452f91");
		m_md5s[String(UTF8("shaders/sources/lava.xml"))] =
			get_md5("8b157949a1e22c220653722246834f11");
		m_md5s[String(
			UTF8("shaders/sources/parallax_slope_mapping.xml"))] =
				get_md5("aba37947f82a096a2a379d9436203dd3");
		m_md5s[String(UTF8("shaders/sources/exp_shadow_mapping.xml"))] =
			get_md5("41c99ed71334ef6adcf360abe5ce0cdb");
		m_md5s[String(UTF8("shaders/sources/uv.xml"))] =
			get_md5("4e5c5bc36c053717dd0cbb7f256eea44");
		m_md5s[String(UTF8("shaders/sources/depth_transform.xml"))] =
			get_md5("dc51638064f107a1ea79d4ce5c4e653e");
		m_md5s[String(
			UTF8("shaders/sources/bone_depth_animation.xml"))] =
				get_md5("1f85066c775d453fb5aad127d6f2a8a5");
		m_md5s[String(
			UTF8("shaders/sources/bone_normal_animation.xml"))] =
				get_md5("aaf5cbae8b7d88541c6ccd586f2b6a21");
		m_md5s[String(UTF8("shaders/sources/diffuse_mapping.xml"))] =
			get_md5("d91575d1a76ce266f8836827c29428a3");
		m_md5s[String(UTF8("shaders/sources/shadow_mapping.xml"))] =
			get_md5("bcf60fe0b6641eb7d9c01101d33cf16e");
		m_md5s[String(UTF8("shaders/sources/fog.xml"))] =
			get_md5("7baadd006ca67ea07b4a8729a08e4b03");
		m_md5s[String(UTF8("shaders/sources/shadow_map.xml"))] =
			get_md5("2aa25f02b3e0ce59a70c71f41b6f8889");
		m_md5s[String(UTF8("shaders/sources/no_normal_mapping.xml"))] =
			get_md5("0f09154ae990b9e658df870ed005c9fd");
*/
	}

	FileSystem::~FileSystem() throw()
	{
	}

	String FileSystem::get_file_name(const String &file_name)
	{
		StringTypeVector splits;

		boost::split(splits, file_name.get(), boost::is_any_of(
			UTF8("\\/")), boost::token_compress_on);

		if (splits.size() > 0)
		{
			return String(*splits.rbegin());
		}

		return String(UTF8(""));
	}

	StringTypeVector FileSystem::get_stripped_path(const String &file_name)
	{
		StringTypeVector splits, path;
		Uint32 i, count;

		boost::split(splits, file_name.get(), boost::is_any_of(
			UTF8("\\/")), boost::token_compress_on);

		count = splits.size();

		for (i = 0; i < count; ++i)
		{
			if (splits[i] == "..")
			{
				if (path.size() > 0)
				{
					path.pop_back();
				}
				continue;
			}

			if (splits[i] == ".")
			{
				continue;
			}

			path.push_back(splits[i]);
		}

		return path;
	}

	String FileSystem::get_strip_relative_path(const String &file_name)
	{
		StringTypeVector path;
		StringType result;
		Uint32 i, count;

		path = get_stripped_path(file_name);

		count = path.size();

		if (count == 0)
		{
			return String(UTF8(""));
		}

		result = path[0];

		for (i = 1; i < count; ++i)
		{
			result += '/';
			result += path[i];
		}

		return String(result);
	}

	String FileSystem::get_dir_name(const String &file_name)
	{
		StringTypeVector path;
		StringType result;
		Uint32 i, count;

		path = get_stripped_path(file_name);

		count = path.size();

		if (count < 2)
		{
			return String(UTF8(""));
		}

		count -= 1;
		result = path[0];

		for (i = 1; i < count; ++i)
		{
			result += '/';
			result += path[i];
		}

		return String(result);
	}

	void FileSystem::add_dirs(const Uint16 major, const Uint16 minor,
		const Uint16 release)
	{
		StringStream str;
		StringType config_dir, update_dir, custom_dir;

		config_dir = string_to_utf8(get_path_config_base());

		m_archives.push_back(new DirArchive(String(config_dir)));

		str << major << "_" << minor << "_" << release;

		update_dir = config_dir;
		update_dir += "/update";
		update_dir += str.str();

		m_archives.push_back(new DirArchive(String(update_dir)));

		custom_dir = config_dir;
		custom_dir += "/custom";

		m_archives.push_back(new DirArchive(String(custom_dir)));
	}

	void FileSystem::add_dir(const String &dir_name)
	{
		m_archives.push_back(new DirArchive(dir_name));
	}

	void FileSystem::add_zip(const String &zip_name)
	{
		m_archives.push_back(new ZipFile(zip_name));
	}

	void FileSystem::add_zip(const String &zip_name,
		const Uint8Array16 &md5)
	{
		Uint8Array16 file_md5;

		file_md5 = get_file_md5(zip_name);

		if (md5 != file_md5)
		{
			LOG_WARNING(UTF8("File '%1%' md5 should be '%2%', but "
				"is '%3%'"), zip_name % get_md5_str(md5) %
					get_md5_str(file_md5));
		}

		m_archives.push_back(new ZipFile(zip_name));
	}

	void FileSystem::add_zip(const String &zip_name,
		const StringType &md5)
	{
		Uint8Array16 file_md5;

		file_md5 = get_file_md5(zip_name);

		if (md5 != get_md5_str(file_md5))
		{
			LOG_WARNING(UTF8("File '%1%' md5 should be '%2%', but "
				"is '%3%'"), zip_name % md5 %
					get_md5_str(file_md5));
		}

		m_archives.push_back(new ZipFile(zip_name));
	}

	bool FileSystem::remove_archive(const String &name)
	{
		AbstractArchiveVector::iterator it, end;

		end = m_archives.end();

		for (it = m_archives.begin(); it != end; ++it)
		{
			if (it->get_name() == name)
			{
				m_archives.release(it);

				return true;
			}
		}

		return false;
	}

	bool FileSystem::get_file_readable(const String &file_name)
	{
		AbstractArchiveVector::const_reverse_iterator it, end;
		String name;

		name = get_strip_relative_path(file_name);

		end = m_archives.rend();

		for (it = m_archives.rbegin(); it != end; ++it)
		{
			if (it->get_has_file(file_name))
			{
				return true;
			}
		}

		return false;
	}

	bool FileSystem::get_file_if_readable(const String &file_name,
		ReaderSharedPtr &reader)
	{
		AbstractArchiveVector::const_reverse_iterator it, end;
		String name;

		name = get_strip_relative_path(file_name);

		end = m_archives.rend();

		for (it = m_archives.rbegin(); it != end; ++it)
		{
			if (it->get_has_file(file_name))
			{
				reader = it->get_file(file_name);

				return true;
			}
		}

		return false;
	}

	ReaderSharedPtr FileSystem::get_file(const String &file_name)
	{
		AbstractArchiveVector::const_reverse_iterator it, end;
		StringUint8Array16Map::const_iterator found;
		ReaderSharedPtr reader;
		Uint8Array16 md5;
		String name;

		name = String(get_strip_relative_path(file_name));

		end = m_archives.rend();

		for (it = m_archives.rbegin(); it != end; ++it)
		{
			if (it->get_has_file(name))
			{
				reader = it->get_file(name);

				found = m_md5s.find(name);

				if (found == m_md5s.end())
				{
					return reader;
				}

				md5 = get_file_md5(reader->get_buffer());

				if (md5 == found->second)
				{
					return reader;
				}

				LOG_WARNING(UTF8("File '%1%/%2%' had md5 "
					"%3% but %4% expected"), it->get_name()
					% name % get_md5_str(md5) %
					get_md5_str(found->second));
			}
		}

		EL_THROW_EXCEPTION(FileNotFoundException()
			<< boost::errinfo_file_name(file_name));
	}

	String FileSystem::get_file_string(const String &file_name)
	{
		ReaderSharedPtr reader;
		String str;

		reader = get_file(file_name);

		str = reader->read_utf8_string(reader->get_size());

		return str;
	}

	String FileSystem::get_file_name_without_extension(
		const String &file_name)
	{
		StringTypeVector path;
		StringType result;
		std::size_t pos;
		Uint32 i, index, count;

		path = get_stripped_path(file_name);

		count = path.size();

		if (count == 0)
		{
			return String(UTF8(""));
		}

		index = count - 1;

		pos = path[index].rfind(UTF8("."));

		if (pos != std::string::npos)
		{
			path[index].erase(path[index].begin() + pos,
				path[index].end());
		}

		result = path[0];

		for (i = 1; i < count; ++i)
		{
			result += '/';
			result += path[i];
		}

		return String(result);
	}

}
