/****************************************************************************
 *            texture.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texture.hpp"
#include "image.hpp"
#include "exceptions.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	Texture::Texture(const String &name, const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmaps,
		const Uint16 samples, const TextureFormatType format,
		const TextureTargetType target): AbstractRenderTarget(name,
			width, height, samples, format),
		m_anisotropic_filter(16.0f), m_texture_id(0),
		m_depth(depth), m_size(0), m_target(target),
		m_mag_filter(tft_linear), m_min_filter(tft_linear),
		m_mipmap_filter(tmt_linear), m_wrap_s(twt_repeat),
		m_wrap_t(twt_repeat), m_wrap_r(twt_repeat),
		m_mipmap_count(mipmaps), m_used_mipmaps(mipmaps),
		m_rebuild(true)
	{
		assert(!get_name().get().empty());
	}

	Texture::~Texture() noexcept
	{
		unload();
	}

	GLenum Texture::get_min_filter(const TextureFilterType texture_filter,
		const TextureMipmapType texture_mipmap)
	{
		switch (texture_mipmap)
		{
			case tmt_none:
				return texture_filter;
			case tmt_nearest:
				switch (texture_filter)
				{
					case tft_nearest:
						return GL_NEAREST_MIPMAP_NEAREST;
					case tft_linear:
						return GL_LINEAR_MIPMAP_NEAREST;
				}
				/*
				 * We should be never here. If so, it's a
				 * programming error, because we forgot to add
				 * all types to the switch or an invalid int
				 * was used (with a type cast)!
				 */
				VALUE_NOT_IN_SWITCH(texture_filter,
					"TextureFilterType");
			case tmt_linear:
				switch (texture_filter)
				{
					case tft_nearest:
						return GL_NEAREST_MIPMAP_LINEAR;
					case tft_linear:
						return GL_LINEAR_MIPMAP_LINEAR;
				}
				/*
				 * We should be never here. If so, it's a
				 * programming error, because we forgot to add
				 * all types to the switch or an invalid int
				 * was used (with a type cast)!
				 */
				VALUE_NOT_IN_SWITCH(texture_filter,
					"TextureFilterType");
		}
		/*
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(texture_mipmap, "TextureMipmapType");
	}

	void Texture::set_texture_filter()
	{
		glTexParameteri(get_gl_target(), GL_TEXTURE_MAG_FILTER,
			get_mag_filter());

		if (get_use_mipmaps())
		{
			glTexParameteri(get_gl_target(), GL_TEXTURE_MIN_FILTER,
				get_min_filter(get_min_filter(),
				get_mipmap_filter()));

			if ((get_anisotropic_filter() >= 1.0f) &&
				(get_mipmap_filter() != tmt_none))
			{
				glTexParameterf(get_gl_target(),
					GL_TEXTURE_MAX_ANISOTROPY_EXT,
					get_anisotropic_filter());
			}
		}
		else
		{
			glTexParameteri(get_gl_target(), GL_TEXTURE_MIN_FILTER,
				get_min_filter());
		}
	}

	void Texture::set_texture_wrap()
	{
		switch (get_target())
		{
			case ttt_texture_3d:
			case ttt_texture_2d_array:
			case ttt_texture_cube_map_array:
				glTexParameteri(get_gl_target(),
					GL_TEXTURE_WRAP_R, get_wrap_r());
			case ttt_texture_2d:
			case ttt_texture_1d_array:
			case ttt_texture_cube_map:
			case ttt_texture_rectangle:
				glTexParameteri(get_gl_target(),
					GL_TEXTURE_WRAP_T, get_wrap_t());
			case ttt_texture_1d:
				glTexParameteri(get_gl_target(),
					GL_TEXTURE_WRAP_S, get_wrap_s());
				return;
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
				return;
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an invalid int
		 * was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(get_target(), UTF8("TextureTargetType"));
	}

	void Texture::set_texture_parameter()
	{
		if (get_target() == ttt_texture_rectangle)
		{
			if (get_use_mipmaps())
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("Can't use "
						"mipmaps for ractangle "
						"textures")));
			}

			if (get_wrap_s() != twt_clamp)
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("Only valid "
						"wrapping mode for ractangle "
						"textures is clamp")));
			}

			if (get_wrap_t() != twt_clamp)
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("Only valid "
						"wrapping mode for ractangle "
						"textures is clamp")));
			}
		}

		set_texture_filter();
		set_texture_wrap();

		if (TextureFormatUtil::get_depth(get_format()))
		{
			glTexParameteri(get_gl_target(),
				GL_TEXTURE_COMPARE_MODE,
				GL_COMPARE_R_TO_TEXTURE);
			glTexParameteri(get_gl_target(),
				GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		}
	}

	void Texture::set_texture_image_1d(const Uint32 width,
		const Uint16 mipmap, const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint64 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(mipmap);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 1d texture '%2%' with width %3% using %4%compressed "
			"data from image '%5%' and target '%6%'."), mipmap %
			get_name() % width %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			image->get_name());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage1D(get_gl_target(), mipmap,
				get_gl_format(), width, 0, size,
				image->get_data(0, mipmap));
		}
		else
		{
			glTexImage1D(get_gl_target(), mipmap, get_gl_format(),
				width, 0, image_format,
				image_type, image->get_data(0, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_2d(const Uint32 width,
		const Uint32 height, const Uint16 mipmap,
		const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint64 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(mipmap);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 2d texture '%2%' with width %3% and height %4% "
			"using %5%compressed data from image '%6%' and target "
			"'%7%'."), mipmap %  get_name() % width % height %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			image->get_name());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage2D(get_gl_target(), mipmap,
				get_gl_format(), width, height,
				0, size, image->get_data(0, mipmap));
		}
		else
		{
			glTexImage2D(get_gl_target(), mipmap, get_gl_format(),
				width, height, 0, image_format, image_type,
				image->get_data(0, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_3d(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap,
		const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(mipmap);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 3d texture '%2%' with width %3%, height %4% and "
			"depth %5% using %6%compressed data and target '%7%' "
			"from image '%8%'."), mipmap % get_name() % width %
			height % depth % (compressed ? UTF8("") : UTF8("un")) %
			get_target() % image->get_name());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage3D(get_gl_target(), mipmap,
				get_gl_format(), width, height,
				depth, 0, size, image->get_data(0, mipmap));
		}
		else
		{
			glTexImage3D(get_gl_target(), mipmap, get_gl_format(),
				width, height, depth, 0, image_format,
				image_type, image->get_data(0, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map(const Uint32 width,
		const Uint32 height, const Uint16 mipmap,
		const ImageSharedPtr &image)
	{
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" cube map texture '%2%' with width %3% and height %4%"
			" using %5%compressed data and target '%6%' from image"
			" '%7%'."), mipmap % get_name() % width % height %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			image->get_name());

		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_positive_x, image);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_negative_x, image);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_positive_y, image);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_negative_y, image);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_positive_z, image);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_negative_z, image);
	}

	void Texture::set_texture_image_cube_map_face(const Uint32 width,
		const Uint32 height, const Uint16 mipmap,
		const CubeMapFaceType face, const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(mipmap);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage2D(face, mipmap, get_gl_format(),
				width, height, 0, size,
				image->get_data(face, mipmap));
		}
		else
		{
			glTexImage2D(face, mipmap, get_gl_format(), width,
				height, 0, image_format, image_type,
				image->get_data(face, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap,
		const ImageSharedPtr &image)
	{
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" cube map array texture '%2%' with width %3%, height "
			"%4% and depth %5% using %6%compressed data and target"
			" '%7%' from image '%7%'."), mipmap % get_name() %
			width % height % depth %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			image->get_name());

		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_positive_x, image);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_negative_x, image);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_positive_y, image);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_negative_y, image);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_positive_z, image);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_negative_z, image);
	}

	void Texture::set_texture_image_cube_map_face(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap,
		const CubeMapFaceType face, const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(mipmap);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage3D(face, mipmap, get_gl_format(),
				width, height, depth, 0, size,
				image->get_data(face, mipmap));
		}
		else
		{
			glTexImage3D(face, mipmap, get_gl_format(), width,
				height, depth, 0, image_format, image_type,
				image->get_data(face, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_2d_layer(const Uint32 width,
		const Uint32 height, const Uint32 layer, const Uint16 mipmap,
		const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(mipmap);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 2d array texture '%2%' with width %3%, height %4% and"
			" layer %5% using %6%compressed data and target '%7%' "
			"from image '%7%'."), mipmap %  get_name() % width %
			height % layer % (compressed ? UTF8("") : UTF8("un")) %
			get_target() % image->get_name());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage3D(get_gl_target(), mipmap, 0, 0,
				layer, width, height, 1, get_gl_format(), size,
				image->get_data(0, mipmap));
		}
		else
		{
			glTexSubImage3D(get_gl_target(), mipmap, 0, 0, layer,
				width, height, 1, image_format, image_type,
				image->get_data(0, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_1d(const Uint32 width,
		const Uint16 texture_mipmap, const Uint16 image_mipmap, 
		const ImageSharedPtr &image,
		const glm::uvec3 &texture_position,
		const glm::uvec3 &image_position)
	{
		GLenum image_format, image_type;
		Uint64 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(width, 1, 1);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 1d texture '%2%' with width %3% using %4%compressed "
			"data and target '%5%' from image mipmap %6% '%7%'."),
			texture_mipmap % get_name() % width %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			image_mipmap % image->get_name());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage1D(get_gl_target(),
				texture_mipmap, texture_position[0], width,
				get_gl_format(), size, image->get_pixel_data(
				image_position[0], 0, 0, 0, image_mipmap));
		}
		else
		{
			glTexSubImage1D(get_gl_target(), texture_mipmap,
				texture_position[0], width, image_format,
				image_type, image->get_pixel_data(
				image_position[0], 0, 0, 0, image_mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_2d(const Uint32 width,
		const Uint32 height, const Uint16 texture_mipmap,
		const Uint16 image_mipmap, const ImageSharedPtr &image,
		const glm::uvec3 &texture_position,
		const glm::uvec3 &image_position)
	{
		GLenum image_format, image_type;
		Uint64 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(width, height, 1);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 2d texture '%2%' with width %3% and height %4% using"
			" %5%compressed data and target '%7%' from image "
			"mipmap %6% '%7%'."), texture_mipmap % get_name() %
			width % height % (compressed ? UTF8("") : UTF8("un")) %
			get_target() % image_mipmap % image->get_name());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage2D(get_gl_target(),
				texture_mipmap, texture_position[0],
				texture_position[1], width, height,
				get_gl_format(), size,
				image->get_pixel_data(image_position[0],
				image_position[1], 0, 0, image_mipmap));
		}
		else
		{
			glTexSubImage2D(get_gl_target(), texture_mipmap,
				texture_position[0], texture_position[1],
				width, height, image_format, image_type,
				image->get_pixel_data(image_position[0],
				image_position[1], 0, 0, image_mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_3d(const Uint32 width,
		const Uint32 height, const Uint32 depth,
		const Uint16 texture_mipmap, const Uint16 image_mipmap,
		const ImageSharedPtr &image,
		const glm::uvec3 &texture_position,
		const glm::uvec3 &image_position)
	{
		GLenum image_format, image_type;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(width, height, depth);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 3d texture '%2%' with width %3%, height %4% and depth"
			" %5% using %6%compressed data and target '%7%' from "
			"image mipmap %8% '%9%'."), texture_mipmap % get_name()
			% width % height % depth %
			(compressed ? UTF8("") : UTF8("un")) % get_target()
			% image_mipmap % image->get_name());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage3D(get_gl_target(),
				texture_mipmap, texture_position[0],
				texture_position[1], texture_position[2],
				width, height, depth, get_gl_format(), size,
				image->get_pixel_data(image_position[0],
				image_position[1], image_position[2], 0,
				image_mipmap));
		}
		else
		{
			glTexSubImage3D(get_gl_target(), texture_mipmap,
				texture_position[0], texture_position[1],
				texture_position[2], width, height, depth,
				image_format, image_type,
				image->get_pixel_data(image_position[0],
				image_position[1], image_position[2], 0,
				image_mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map(const Uint32 width,
		const Uint32 height, const Uint16 texture_mipmap,
		const Uint16 image_mipmap, const ImageSharedPtr &image,
		const glm::uvec3 &texture_position,
		const glm::uvec3 &image_position)
	{
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" cube map texture '%2%' with width %3% and height %4%"
			" using %5%compressed data and target '%6%' from image"
			" mipmap %7% '%8%'."), texture_mipmap % get_name() %
			width % height % (compressed ? UTF8("") : UTF8("un")) %
			get_target() % image_mipmap % image->get_name());

		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, cmft_positive_x, image, texture_position,
			image_position);
		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, cmft_negative_x, image, texture_position,
			image_position);
		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, cmft_positive_y, image, texture_position,
			image_position);
		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, cmft_negative_y, image, texture_position,
			image_position);
		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, cmft_positive_z, image, texture_position,
			image_position);
		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, cmft_negative_z, image, texture_position,
			image_position);
	}

	void Texture::set_texture_image_cube_map_face(const Uint32 width,
		const Uint32 height, const Uint16 texture_mipmap,
		const Uint16 image_mipmap, const CubeMapFaceType face,
		const ImageSharedPtr &image, const glm::uvec3 &texture_position,
		const glm::uvec3 &image_position)
	{
		GLenum image_format, image_type;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(width, height, 1);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage2D(face, texture_mipmap,
				texture_position[0], texture_position[1],
				width, height, get_gl_format(), size,
				image->get_pixel_data(image_position[0],
				image_position[1], 0, 0, image_mipmap));
		}
		else
		{
			glTexSubImage2D(face, texture_mipmap,
				texture_position[0], texture_position[1],
				width, height, image_format, image_type,
				image->get_pixel_data(image_position[0],
				image_position[1], 0, 0, image_mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map(const Uint32 width,
		const Uint32 height, const Uint32 depth,
		const Uint16 texture_mipmap, const Uint16 image_mipmap,
		const ImageSharedPtr &image,
		const glm::uvec3 &texture_position,
		const glm::uvec3 &image_position)
	{
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" cube map array texture '%2%' with width %3%, height "
			"%4% and depth %5% using %6%compressed data and target"
			" '%7%' from image mipmap %8% '%9%'."), texture_mipmap
			% get_name() % width % height % depth %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			image_mipmap % image->get_name());

		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, depth, cmft_positive_x, image,
			texture_position, image_position);
		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, depth, cmft_negative_x, image,
			texture_position, image_position);
		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, depth, cmft_positive_y, image,
			texture_position, image_position);
		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, depth, cmft_negative_y, image,
			texture_position, image_position);
		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, depth, cmft_positive_z, image,
			texture_position, image_position);
		set_texture_image_cube_map_face(texture_mipmap, image_mipmap,
			width, height, depth, cmft_negative_z, image,
			texture_position, image_position);
	}

	void Texture::set_texture_image_cube_map_face(const Uint32 width,
		const Uint32 height, const Uint32 depth,
		const Uint16 texture_mipmap, const Uint16 image_mipmap,
		const CubeMapFaceType face, const ImageSharedPtr &image,
		const glm::uvec3 &texture_position,
		const glm::uvec3 &image_position)
	{
		GLenum image_format, image_type;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_size(width, height, depth);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage3D(face, texture_mipmap,
				texture_position[0], texture_position[1],
				texture_position[2], width, height, depth,
				get_gl_format(), size,
				image->get_pixel_data(image_position[0],
				image_position[1], image_position[2], 0,
				image_mipmap));
		}
		else
		{
			glTexSubImage3D(face, texture_mipmap,
				texture_position[0], texture_position[1],
				texture_position[2], width, height, depth,
				image_format, image_type,
				image->get_pixel_data(image_position[0],
				image_position[1], image_position[2], 0,
				image_mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_1d(const Uint32 width,
		const Uint16 mipmap)
	{
		Uint32 size;
		GLenum format, type;
		bool compressed;

		TextureFormatUtil::get_source_format(get_format(), format,
			type);

		compressed = TextureFormatUtil::get_compressed(get_format());
		size = Image::get_size(width, 1, 1, get_format());

		LOG_DEBUG(lt_texture, UTF8("Initializing texture mipmap level "
			"%1% of 1d texture '%2%' with width %3% using "
			"%4%compressed format and target '%5%'."), mipmap %
			get_name() % width %
			(compressed ? UTF8("") : UTF8("un")) % get_target());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage1D(get_gl_target(), mipmap,
				get_gl_format(), width, 0, size, 0);
		}
		else
		{
			glTexImage1D(get_gl_target(), mipmap, get_gl_format(),
				width, 0, format, type, 0);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_2d(const Uint32 width,
		const Uint32 height, const Uint16 mipmap)
	{
		Uint32 size;
		GLenum format, type;
		bool compressed;

		TextureFormatUtil::get_source_format(get_format(), format,
			type);

		compressed = TextureFormatUtil::get_compressed(get_format());
		size = Image::get_size(width, height, 1, get_format());

		LOG_DEBUG(lt_texture, UTF8("Initializing texture mipmap level "
			"%1% of 2d texture '%2%' with width %3% and height %4%"
			" using %5%compressed format and target '%6%'."),
			mipmap % get_name() % width % height %
			(compressed ? UTF8("") : UTF8("un")) % get_target());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage2D(get_gl_target(), mipmap,
				get_gl_format(), width, height, 0, size, 0);
		}
		else
		{
			glTexImage2D(get_gl_target(), mipmap, get_gl_format(),
				width, height, 0, format, type, 0);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_3d(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap)
	{
		Uint32 size;
		GLenum format, type;
		bool compressed;

		TextureFormatUtil::get_source_format(get_format(), format,
			type);

		compressed = TextureFormatUtil::get_compressed(get_format());
		size = Image::get_size(width, height, depth, get_format());

		LOG_DEBUG(lt_texture, UTF8("Initializing texture mipmap level "
			"%1% of 3d texture '%2%' with width %3%, height %4% "
			"and depth %5% using %6%compressed format and target "
			"'%7%'."), mipmap % get_name() % width % height % depth
			% (compressed ? UTF8("") : UTF8("un")) % get_target());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage3D(get_gl_target(), mipmap,
				get_gl_format(), width, height,
				depth, 0, size, 0);
		}
		else
		{
			glTexImage3D(get_gl_target(), mipmap, get_gl_format(),
				width, height, depth, 0, format, type, 0);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map(const Uint32 width,
		const Uint32 height, const Uint16 mipmap)
	{
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(get_format());

		LOG_DEBUG(lt_texture, UTF8("Initializing texture mipmap level "
			"%1% of cube map texture '%2%' with width %3% and "
			"height %4% using %5%compressed format and target "
			"'%6%'."), mipmap % get_name() % width % height %
			(compressed ? UTF8("") : UTF8("un")) % get_target());

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_positive_x);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_negative_x);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_positive_y);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_negative_y);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_positive_z);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_negative_z);
	}

	void Texture::set_texture_image_cube_map_face(const Uint32 width,
		const Uint32 height, const Uint16 mipmap,
		const CubeMapFaceType face)
	{
		Uint32 size;
		GLenum format, type;
		bool compressed;

		CHECK_GL_ERROR_NAME(get_name());

		TextureFormatUtil::get_source_format(get_format(), format,
			type);

		compressed = TextureFormatUtil::get_compressed(get_format());
		size = Image::get_size(width, height, 1, get_format());

		if (compressed)
		{
			glCompressedTexImage2D(get_gl_target(), mipmap,
				get_gl_format(), width, height, 0, size, 0);
		}
		else
		{
			glTexImage2D(get_gl_target(), mipmap, get_gl_format(),
				width, height, 0, format, type, 0);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap)
	{
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(get_format());

		LOG_DEBUG(lt_texture, UTF8("Initializing texture mipmap level "
			"%1% of cube map texture '%2%' with width %3%, height "
			"%4% and depth %5% using %6%compressed format and "
			"target '%7%'."), mipmap % get_name() % width % height
			% depth % (compressed ? UTF8("") : UTF8("un")) %
			get_target());

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_positive_x);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_negative_x);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_positive_y);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_negative_y);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_positive_z);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_negative_z);
	}

	void Texture::set_texture_image_cube_map_face(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap,
		const CubeMapFaceType face)
	{
		Uint32 size;
		GLenum format, type;
		bool compressed;

		CHECK_GL_ERROR_NAME(get_name());

		TextureFormatUtil::get_source_format(get_format(), format,
			type);

		compressed = TextureFormatUtil::get_compressed(get_format());
		size = Image::get_size(width, height, depth, get_format());

		if (compressed)
		{
			glCompressedTexImage3D(face, mipmap, get_gl_format(),
				width, height, depth, 0, size, 0);
		}
		else
		{
			glTexImage3D(face, mipmap, get_gl_format(), width,
				height, depth, 0, format, type, 0);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_2d_multisample()
	{
		CHECK_GL_ERROR_NAME(get_name());

		glTexImage2DMultisample(get_gl_target(), get_samples(),
			get_gl_format(), get_width(), get_height(), false);

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_3d_multisample()
	{
		CHECK_GL_ERROR_NAME(get_name());

		glTexImage3DMultisample(get_gl_target(), get_samples(),
			get_gl_format(), get_width(), get_height(),
			get_depth(), false);

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_image(const ImageSharedPtr &image)
	{
		Uint32 width, height, depth, mip, level, mipmaps, layer;

		CHECK_GL_ERROR();

		m_rebuild |= (get_width() != image->get_width()) ||
			(get_height() != image->get_height()) ||
			(get_depth() != image->get_depth());
		set_width(image->get_width());
		set_height(image->get_height());
		set_depth(image->get_depth());

		build_texture_id();
		bind();

		CHECK_GL_ERROR_NAME(get_name());

		if (image->has_mipmaps())
		{
			mipmaps = std::min(image->get_mipmap_count(),
				get_mipmap_count());
			level = mipmaps + 1;

			glTexParameteri(get_gl_target(), GL_TEXTURE_MAX_LEVEL,
				mipmaps);
		}
		else
		{
			if (get_mipmap_count() > 0)
			{
				glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

				glTexParameteri(get_gl_target(),
					GL_TEXTURE_MAX_LEVEL,
					get_mipmap_count());

				glTexParameteri(get_gl_target(),
					GL_GENERATE_MIPMAP, GL_TRUE);
				mipmaps = get_mipmap_count();
			}
			else
			{
				mipmaps = 0;
			}

			level = 1;
		}

		m_used_mipmaps = mipmaps;

		width = get_width();
		height = get_height();
		depth = get_depth();
		layer = get_depth();

		LOG_DEBUG(lt_texture, UTF8("Loading texture %1% of format "
			"%2% and target '%3%'."), get_name() % get_format() %
			get_target());

		for (mip = 0; mip < level; mip++)
		{
			switch (get_target())
			{
				case ttt_texture_1d:
					set_texture_image_1d(width, mip, image);
					break;
				case ttt_texture_1d_array:
					set_texture_image_2d(width, layer,
						mip, image);
					break;
				case ttt_texture_2d:
				case ttt_texture_rectangle:
					set_texture_image_2d(width, height,
						mip, image);
					break;
				case ttt_texture_3d:
					set_texture_image_3d(width, height,
						depth, mip, image);
					break;
				case ttt_texture_2d_array:
					set_texture_image_3d(width, height,
						layer, mip, image);
					break;
				case ttt_texture_cube_map:
					set_texture_image_cube_map(width,
						height, mip, image);
					break;
				case ttt_texture_cube_map_array:
					set_texture_image_cube_map(width,
						height, layer, mip, image);
					break;
				case ttt_texture_2d_multisample:
				case ttt_texture_2d_multisample_array:
					EL_THROW_EXCEPTION(
						InvalidParameterException()
						<< errinfo_message(UTF8("Can't"
							" set image of "
							"multisample "
							"texture")));
					break;
			}
			if (width > 1)
			{
				width = width / 2;
			}
			if (height > 1)
			{
				height = height / 2;
			}
			if (depth > 1)
			{
				depth = depth / 2;
			}
		}

		calc_size();

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_parameter();

		CHECK_GL_ERROR_NAME(get_name());

		unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_images(const ImageSharedPtrVector &images)
	{
		Uint32 layer, i, w, h, mip, level, mipmaps;

		CHECK_GL_ERROR();

		RANGE_CECK_MIN(images.size(), 1, UTF8("not enough images."));

		m_rebuild |= (get_width() != images[0]->get_width()) ||
			(get_height() != images[0]->get_height()) ||
			(get_depth() != images.size()) ||
			(get_target() != ttt_texture_2d_array);
		set_width(images[0]->get_width());
		set_height(images[0]->get_height());
		set_target(ttt_texture_2d_array);
		set_depth(images.size());

		mipmaps = images[0]->get_mipmap_count();

		BOOST_FOREACH(const ImageSharedPtr &image, images)
		{
			if (get_width() != image->get_width())
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("wrong width"))
					<< errinfo_name(get_name())
					<< errinfo_item_name(image->get_name())
					<< errinfo_value(image->get_width())
					<< errinfo_expected_value(get_width(
						)));
			}

			if (get_height() != image->get_height())
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("wrong height"))
					<< errinfo_name(get_name())
					<< errinfo_item_name(image->get_name())
					<< errinfo_value(image->get_height())
					<< errinfo_expected_value(get_height(
						)));
			}

			if (mipmaps > image->get_mipmap_count())
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("not enought "
						"mipmap levels"))
					<< errinfo_name(get_name())
					<< errinfo_item_name(image->get_name())
					<< errinfo_value(image->get_height())
					<< errinfo_expected_value(get_height(
						)));
			}
		}

		CHECK_GL_ERROR_NAME(get_name());

		build_texture_id();
		bind();

		if (images[0]->has_mipmaps())
		{
			mipmaps = std::min(images[0]->get_mipmap_count(),
				get_mipmap_count());
			level = mipmaps + 1;

			glTexParameteri(get_gl_target(), GL_TEXTURE_MAX_LEVEL,
				mipmaps);
		}
		else
		{
			if (get_mipmap_count() > 0)
			{
				glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

				glTexParameteri(get_gl_target(),
					GL_TEXTURE_MAX_LEVEL,
					get_mipmap_count());

				glTexParameteri(get_gl_target(),
					GL_GENERATE_MIPMAP, GL_TRUE);

				mipmaps = get_mipmap_count();
			}
			else
			{
				mipmaps = 0;
			}

			level = 1;
		}

		m_used_mipmaps = mipmaps;

		if (mipmaps > 0)
		{
			glTexParameteri(get_gl_target(), GL_TEXTURE_MAX_LEVEL,
				get_used_mipmaps());
		}
		else
		{
			if (get_mipmap_count() > 0)
			{
				glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

				glTexParameteri(get_gl_target(),
					GL_TEXTURE_MAX_LEVEL,
					get_mipmap_count());

				glTexParameteri(get_gl_target(),
					GL_GENERATE_MIPMAP, GL_TRUE);
			}
		}

		CHECK_GL_ERROR_NAME(get_name());

		w = get_width();
		h = get_height();
		layer = get_depth();

		for (mip = 0; mip < level; mip++)
		{
			set_texture_image_3d(mip, w, h, layer);

			if (w > 1)
			{
				w = w / 2;
			}

			if (h > 1)
			{
				h = h / 2;
			}
		}

		for (i = 0; i < layer; ++i)
		{
			w = get_width();
			h = get_height();

			for (mip = 0; mip < level; mip++)
			{
				set_texture_image_2d_layer(mip, w, h, i,
					images[i]);

				if (w > 1)
				{
					w = w / 2;
				}

				if (h > 1)
				{
					h = h / 2;
				}
			}
		}

		calc_size();

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_parameter();

		CHECK_GL_ERROR_NAME(get_name());

		unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::sub_texture(const Uint16 mipmap,
		const ImageSharedPtr &image, const glm::uvec3 &position)
	{
		glm::uvec3 zero = glm::uvec3(0);

		sub_texture(mipmap, 0, image, position, zero,
			image->get_sizes());
	}

	void Texture::sub_texture(const Uint16 texture_mipmap,
		const Uint16 image_mipmap, const ImageSharedPtr &image,
		const glm::uvec3 &texture_position,
		const glm::uvec3 &image_position, const glm::uvec3 &size)
	{
		CHECK_GL_ERROR();

		bind();

		CHECK_GL_ERROR_NAME(get_name());

		switch (get_target())
		{
			case ttt_texture_1d:
				set_texture_image_1d(size[0], texture_mipmap,
					image_mipmap, image, texture_position,
					image_position);
				break;
			case ttt_texture_2d:
			case ttt_texture_1d_array:
			case ttt_texture_rectangle:
				set_texture_image_2d(size[0], size[1],
					texture_mipmap, image_mipmap, image,
					texture_position, image_position);
				break;
			case ttt_texture_3d:
			case ttt_texture_2d_array:
				set_texture_image_3d(size[0], size[1], size[2],
					texture_mipmap, image_mipmap, image,
					texture_position, image_position);
				break;
			case ttt_texture_cube_map:
				set_texture_image_cube_map(size[0], size[1],
					texture_mipmap, image_mipmap, image,
					texture_position, image_position);
				break;
			case ttt_texture_cube_map_array:
				set_texture_image_cube_map(size[0], size[1],
					size[2], texture_mipmap, image_mipmap,
					image, texture_position,
					image_position);
				break;
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("Can't set "
						"subimage of multisample "
						"texture")));
				break;
		}

		CHECK_GL_ERROR_NAME(get_name());

		unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::calc_size()
	{
		Uint32 width, height, depth, mip, faces, layer, samples;
		Uint32 used_mipmaps;

		width = get_width();
		height = get_height();
		depth = get_depth();
		samples = get_samples();
		faces = 1;
		layer = 1;
		used_mipmaps = get_used_mipmaps();

		m_size = 0;

		switch (get_target())
		{
			case ttt_texture_1d:
				height = 1;
				depth = 1;
				samples = 1;
				break;
			case ttt_texture_2d:
			case ttt_texture_rectangle:
				depth = 1;
				samples = 1;
				break;
			case ttt_texture_1d_array:
				layer = depth;
				height = 1;
				depth = 1;
				samples = 1;
				break;
			case ttt_texture_3d:
				samples = 1;
				break;
			case ttt_texture_2d_array:
				layer = depth;
				depth = 1;
				samples = 1;
				break;
			case ttt_texture_cube_map:
				depth = 1;
				faces = 6;
				samples = 1;
				break;
			case ttt_texture_cube_map_array:
				layer = depth;
				depth = 1;
				faces = 6;
				samples = 1;
				break;
			case ttt_texture_2d_multisample:
				depth = 1;
				used_mipmaps = 1;
				break;
			case ttt_texture_2d_multisample_array:
				layer = depth;
				depth = 1;
				used_mipmaps = 1;
				break;
		}

		for (mip = 0; mip < (used_mipmaps + 1); mip++)
		{
			m_size += (width * height * depth * layer * faces *
				samples * 
				TextureFormatUtil::get_size(get_format()) + 7)
				/ 8;
			if ((width == 1) && (height == 1) && (depth == 1))
			{
				break;
			}
			if (width > 1)
			{
				width = width / 2;
			}
			if (height > 1)
			{
				height = height / 2;
			}
			if (depth > 1)
			{
				depth = depth / 2;
			}
		}
	}

	void Texture::init(const Uint32 width, const Uint32 height,
		const Uint32 depth, const Uint16 mipmaps,
		const Uint16 samples)
	{
		Uint32 w, h, d, mip, layer;

		CHECK_GL_ERROR();

		m_rebuild |= (get_width() != width) || (get_height() != height)
			|| (get_depth() != depth) ||
			(get_used_mipmaps() != mipmaps) ||
			(get_samples() != samples);
		set_width(width);
		set_height(height);
		set_depth(depth);
		set_samples(samples);

		CHECK_GL_ERROR_NAME(get_name());

		build_texture_id();
		bind();

		CHECK_GL_ERROR_NAME(get_name());

		m_used_mipmaps = mipmaps;

		w = get_width();
		h = get_height();
		d = get_depth();
		layer = get_depth();

		for (mip = 0; mip < (get_used_mipmaps() + 1); mip++)
		{
			switch (get_target())
			{
				case ttt_texture_1d:
					set_texture_image_1d(w, mip);
					break;
				case ttt_texture_2d:
				case ttt_texture_1d_array:
				case ttt_texture_rectangle:
					set_texture_image_2d(w, h, mip);
					break;
				case ttt_texture_3d:
					set_texture_image_3d(w, h, d, mip);
					break;
				case ttt_texture_2d_array:
					set_texture_image_3d(w, h, layer, mip);
					break;
				case ttt_texture_cube_map:
					set_texture_image_cube_map(w, h, mip);
					break;
				case ttt_texture_cube_map_array:
					set_texture_image_cube_map(w, h, layer,
						mip);
					break;
				case ttt_texture_2d_multisample:
					set_texture_image_2d_multisample();
					break;
				case ttt_texture_2d_multisample_array:
					set_texture_image_3d_multisample();
					break;
			}
			if (w > 1)
			{
				w = w / 2;
			}
			if (h > 1)
			{
				h = h / 2;
			}
			if (d > 1)
			{
				d = d / 2;
			}
		}

		calc_size();

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_parameter();

		CHECK_GL_ERROR_NAME(get_name());

		unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::attach_ext(const GLenum attachment, const Uint32 level,
		const Uint32 layer)
	{
		CHECK_GL_ERROR();

		switch (get_target())
		{
			case ttt_texture_1d:
				glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT,
					attachment, get_gl_target(),
					get_texture_id(), level);
				break;
			case ttt_texture_2d:
			case ttt_texture_rectangle:
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
					attachment, get_gl_target(),
					get_texture_id(), level);
				break;
			case ttt_texture_3d:
				glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT,
					attachment, get_gl_target(),
					get_texture_id(), level, layer);
				break;
			case ttt_texture_1d_array:
			case ttt_texture_2d_array:
			case ttt_texture_cube_map:
			case ttt_texture_cube_map_array:
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
			default:
				assert(false);
				break;
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::attach(const GLenum attachment, const Uint32 level,
		const Uint32 layer)
	{
		CHECK_GL_ERROR();

		switch (get_target())
		{
			case ttt_texture_1d:
				glFramebufferTexture1D(GL_FRAMEBUFFER,
					attachment, get_gl_target(),
					get_texture_id(), level);
				break;
			case ttt_texture_2d:
			case ttt_texture_rectangle:
				glFramebufferTexture2D(GL_FRAMEBUFFER,
					attachment, get_gl_target(),
					get_texture_id(), level);
				break;
			case ttt_texture_3d:
				glFramebufferTexture3D(GL_FRAMEBUFFER,
					attachment, get_gl_target(),
					get_texture_id(), level, layer);
				break;
			case ttt_texture_1d_array:
			case ttt_texture_2d_array:
				glFramebufferTextureLayer(GL_FRAMEBUFFER,
					attachment, get_texture_id(), level,
					layer);
				break;
			case ttt_texture_2d_multisample:
				glFramebufferTexture2D(GL_FRAMEBUFFER,
					attachment, get_gl_target(),
					get_texture_id(), 0);
				break;
			case ttt_texture_2d_multisample_array:
				glFramebufferTexture3D(GL_FRAMEBUFFER,
					attachment, get_gl_target(),
					get_texture_id(), 0, layer);
				break;
			case ttt_texture_cube_map:
			case ttt_texture_cube_map_array:
			default:
				assert(false);
				break;
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::attach(const GLenum attachment, const Uint32 level)
	{
		CHECK_GL_ERROR();

		switch (get_target())
		{
			case ttt_texture_3d:
			case ttt_texture_1d_array:
			case ttt_texture_2d_array:
			case ttt_texture_cube_map:
			case ttt_texture_cube_map_array:
				glFramebufferTexture(GL_FRAMEBUFFER,
					attachment, get_texture_id(), level);
				break;
			case ttt_texture_2d_multisample_array:
				glFramebufferTexture(GL_FRAMEBUFFER,
					attachment, get_texture_id(), 0);
				break;
			case ttt_texture_1d:
			case ttt_texture_2d:
			case ttt_texture_2d_multisample:
			case ttt_texture_rectangle:
			default:
				assert(false);
				break;
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::attach(const GLenum attachment,
		const CubeMapFaceType face, const Uint32 level)
	{
		CHECK_GL_ERROR();

		switch (get_target())
		{
			case ttt_texture_cube_map:
				glFramebufferTexture2D(GL_FRAMEBUFFER,
					attachment, face, get_texture_id(),
					level);
				break;
			case ttt_texture_1d:
			case ttt_texture_2d:
			case ttt_texture_1d_array:
			case ttt_texture_rectangle:
			case ttt_texture_3d:
			case ttt_texture_2d_array:
			case ttt_texture_cube_map_array:
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
			default:
				assert(false);
				break;
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::attach(const GLenum attachment,
		const CubeMapFaceType face, const Uint32 level,
		const Uint32 layer)
	{
		CHECK_GL_ERROR();

		switch (get_target())
		{
			case ttt_texture_1d:
			case ttt_texture_2d:
			case ttt_texture_1d_array:
			case ttt_texture_rectangle:
			case ttt_texture_3d:
			case ttt_texture_2d_array:
			case ttt_texture_cube_map:
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
				assert(false);
				break;
			case ttt_texture_cube_map_array:
				glFramebufferTexture3D(GL_FRAMEBUFFER,
					attachment, face, get_texture_id(),
					level, layer);
				break;
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::build_texture_id()
	{
		CHECK_GL_ERROR();

		if (m_rebuild)
		{
			if (m_texture_id != 0)
			{
				glDeleteTextures(1, &m_texture_id);
				m_texture_id = 0;
				m_rebuild = false;
			}
		}

		if (m_texture_id == 0)
		{
			glGenTextures(1, &m_texture_id);
			m_rebuild = false;
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::unload() noexcept
	{
		glDeleteTextures(1, &m_texture_id);

		m_size = 0;
		m_texture_id = 0;
	}

	String Texture::get_str(const TextureFilterType value)
	{
		switch (value)
		{
			case tft_nearest:
				return String(UTF8("nearest"));
			case tft_linear:
				return String(UTF8("linear"));
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(value, UTF8("TextureFilterType"));
	}

	String Texture::get_str(const TextureMipmapType value)
	{
		switch (value)
		{
			case tmt_none:
				return String(UTF8("none"));
			case tmt_nearest:
				return String(UTF8("nearest"));
			case tmt_linear:
				return String(UTF8("linear"));
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(value, UTF8("TextureMipmapType"));
	}

	String Texture::get_str(const TextureWrapType value)
	{
		switch (value)
		{
			case twt_clamp:
				return String(UTF8("clamp"));
			case twt_mirrored_repeat:
				return String(UTF8("mirrored_repeat"));
			case twt_repeat:
				return String(UTF8("repeat"));
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(value, UTF8("TextureWrapType"));
	}

	OutStream& operator<<(OutStream &str, const TextureFilterType value)
	{
		str << Texture::get_str(value);

		return str;
	}

	OutStream& operator<<(OutStream &str, const TextureMipmapType value)
	{
		str << Texture::get_str(value);

		return str;
	}

	OutStream& operator<<(OutStream &str, const TextureWrapType value)
	{
		str << Texture::get_str(value);

		return str;
	}

}
