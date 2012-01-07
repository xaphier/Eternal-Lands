/****************************************************************************
 *            texture.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f8a7405b_ced9_4d49_9ad0_cf51cc1a3dfd
#define	UUID_f8a7405b_ced9_4d49_9ad0_cf51cc1a3dfd

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

/**
 * @file
 * @brief The @c class Texture.
 * This file contains the @c class Texture.
 */
namespace eternal_lands
{

	/**
	 * Types of cube map faces.
	 * @{
	 */
	enum CubeMapFaceType
	{
		cmft_negative_x = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		cmft_negative_y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		cmft_negative_z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		cmft_positive_x = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		cmft_positive_y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		cmft_positive_z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z
	};
	/**
	 * @}
	 */

	/**
	 * Types of texture targets.
	 * @{
	 */
	enum TextureTargetType
	{
		ttt_1d_texture = GL_TEXTURE_1D,
		ttt_2d_texture = GL_TEXTURE_2D,
		ttt_3d_texture = GL_TEXTURE_3D,
		ttt_cube_map_texture = GL_TEXTURE_CUBE_MAP,
		ttt_1d_texture_array = GL_TEXTURE_1D_ARRAY,
		ttt_2d_texture_array = GL_TEXTURE_2D_ARRAY,
		ttt_cube_map_texture_array = GL_TEXTURE_CUBE_MAP_ARRAY,
		ttt_texture_rectangle = GL_TEXTURE_RECTANGLE
	};
	/**
	 * @}
	 */

	/**
	 * Types of texture filter modes.
	 * @{
	 */
	enum TextureFilterType
	{
		tft_nearest = GL_NEAREST,
		tft_linear = GL_LINEAR
	};
	/**
	 * @}
	 */

	/**
	 * Types of texture mipmap modes.
	 * @{
	 */
	enum TextureMipmapType
	{
		tmt_none = 0,
		tmt_nearest = 1,
		tmt_linear = 2
	};
	/**
	 * @}
	 */

	/**
	 * Types of texture wrapping modes.
	 * @{
	 */
	enum TextureWrapType
	{
		twt_clamp = GL_CLAMP_TO_EDGE,
		twt_mirrored_repeat = GL_MIRRORED_REPEAT,
		twt_repeat = GL_REPEAT
	};
	/**
	 * @}
	 */

	/**
	 * @brief @c class for textures.
	 *
	 * @c class for textures.
	 */
	class Texture: public boost::noncopyable
	{
		private:
			const String m_name;
			GLfloat m_anisotropic_filter;
			GLuint m_texture_id;
			Uint32 m_width;
			Uint32 m_height;
			Uint32 m_depth;
			Uint32 m_size;
			TextureFormatType m_format;
			TextureTargetType m_target;
			TextureFilterType m_mag_filter;
			TextureFilterType m_min_filter;
			TextureMipmapType m_mipmap_filter;
			TextureWrapType m_wrap_s;
			TextureWrapType m_wrap_t;
			TextureWrapType m_wrap_r;
			Uint16 m_mipmap_count;
			Uint16 m_used_mipmaps;
			bool m_rebuild;

			void set_texture_image_1d(const Uint16 mipmap,
				const Uint32 width,
				const ImageSharedPtr &image);
			void set_texture_image_2d(const Uint16 mipmap,
				const Uint32 width, const Uint32 height,
				const ImageSharedPtr &image);
			void set_texture_image_3d(const Uint16 mipmap,
				const Uint32 width, const Uint32 height,
				const Uint32 depth,
				const ImageSharedPtr &image);
			void set_texture_image_cube_map(const Uint16 mipmap,
				const Uint32 width, const Uint32 height,
				const ImageSharedPtr &image);
			void set_texture_image_cube_map_face(
				const Uint16 mipmap, const Uint32 width,
				const Uint32 height, const CubeMapFaceType face,
				const ImageSharedPtr &image);
			void set_texture_image_cube_map(const Uint16 mipmap,
				const Uint32 width, const Uint32 height,
				const Uint32 depth,
				const ImageSharedPtr &image);
			void set_texture_image_cube_map_face(
				const Uint16 mipmap, const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const CubeMapFaceType face,
				const ImageSharedPtr &image);

			void set_texture_image_2d_layer(const Uint16 mipmap,
				const Uint32 width, const Uint32 height,
				const Uint32 layer,
				const ImageSharedPtr &image);
			void set_texture_image_1d(const Uint16 texture_mipmap,
				const Uint16 image_mipmap, const Uint32 width,
				const ImageSharedPtr &image,
				const Uint32Array3 &texture_position,
				const Uint32Array3 &image_position);
			void set_texture_image_2d(const Uint16 texture_mipmap,
				const Uint16 image_mipmap, const Uint32 width,
				const Uint32 height,
				const ImageSharedPtr &image,
				const Uint32Array3 &texture_position,
				const Uint32Array3 &image_position);
			void set_texture_image_3d(const Uint16 texture_mipmap,
				const Uint16 image_mipmap, const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const ImageSharedPtr &image,
				const Uint32Array3 &texture_position,
				const Uint32Array3 &image_position);
			void set_texture_image_cube_map(
				const Uint16 texture_mipmap,
				const Uint16 image_mipmap, const Uint32 width,
				const Uint32 height,
				const ImageSharedPtr &image,
				const Uint32Array3 &texture_position,
				const Uint32Array3 &image_position);
			void set_texture_image_cube_map_face(
				const Uint16 texture_mipmap,
				const Uint16 image_mipmap, const Uint32 width,
				const Uint32 height, const CubeMapFaceType face,
				const ImageSharedPtr &image,
				const Uint32Array3 &texture_position,
				const Uint32Array3 &image_position);
			void set_texture_image_cube_map(
				const Uint16 texture_mipmap,
				const Uint16 image_mipmap, const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const ImageSharedPtr &image,
				const Uint32Array3 &texture_position,
				const Uint32Array3 &image_position);
			void set_texture_image_cube_map_face(
				const Uint16 texture_mipmap,
				const Uint16 image_mipmap, const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const CubeMapFaceType face,
				const ImageSharedPtr &image,
				const Uint32Array3 &texture_position,
				const Uint32Array3 &image_position);

			void set_texture_image_1d(const Uint16 mipmap,
				const Uint32 width);
			void set_texture_image_2d(const Uint16 mipmap,
				const Uint32 width, const Uint32 height);
			void set_texture_image_3d(const Uint16 mipmap,
				const Uint32 width, const Uint32 height,
				const Uint32 depth);
			void set_texture_image_cube_map(const Uint16 mipmap,
				const Uint32 width, const Uint32 height);
			void set_texture_image_cube_map_face(
				const Uint16 mipmap, const Uint32 width,
				const Uint32 height,
				const CubeMapFaceType face);
			void set_texture_image_cube_map(const Uint16 mipmap,
				const Uint32 width, const Uint32 height,
				const Uint32 depth);
			void set_texture_image_cube_map_face(
				const Uint16 mipmap, const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const CubeMapFaceType face);

			static GLenum get_min_filter(
				const TextureFilterType texture_filter,
				const TextureMipmapType texture_mipmap);
			void set_texture_filter();
			void set_texture_wrap();
			void set_texture_parameter();
			void build_texture_id();
			void calc_size();

			inline bool get_use_mipmaps() const
			{
				return m_used_mipmaps > 0;
			}

			inline Uint16 get_used_mipmaps() const
			{
				return m_used_mipmaps;
			}
		public:
			inline GLuint get_texture_id() const
			{
				return m_texture_id;
			}

		public:
			Texture(const String &name);
			~Texture() throw();
			void unload() throw();
			static String get_str(const CubeMapFaceType value);
			static String get_str(const TextureTargetType value);
			static String get_str(const TextureFilterType value);
			static String get_str(const TextureMipmapType value);
			static String get_str(const TextureWrapType value);

			inline void bind()
			{
				assert(!m_rebuild);

				glBindTexture(get_target(), get_texture_id());
			}

			inline Uint64 get_size() const
			{
				return m_size;
			}

			inline TextureFormatType get_format() const
			{
				return m_format;
			}

			inline TextureTargetType get_target() const
			{
				return m_target;
			}

			inline TextureFilterType get_mag_filter() const
			{
				return m_mag_filter;
			}

			inline TextureFilterType get_min_filter() const
			{
				return m_min_filter;
			}

			inline TextureMipmapType get_mipmap_filter() const
			{
				return m_mipmap_filter;
			}

			inline GLfloat get_anisotropic_filter() const
			{
				return m_anisotropic_filter;
			}

			inline Uint16 get_mipmap_count() const
			{
				return m_mipmap_count;
			}

			inline TextureWrapType get_wrap_s() const
			{
				return m_wrap_s;
			}

			inline TextureWrapType get_wrap_t() const
			{
				return m_wrap_t;
			}

			inline TextureWrapType get_wrap_r() const
			{
				return m_wrap_r;
			}

			inline Uint32 get_width() const
			{
				return m_width;
			}

			inline Uint32 get_height() const
			{
				return m_height;
			}

			inline Uint32 get_depth() const
			{
				return m_depth;
			}

			inline GLenum get_gl_format() const
			{
				return TextureFormatUtil::get_gl_format(
					get_format());
			}

			inline void set_width(const Uint32 width)
			{
				m_rebuild = m_width != width;
				m_width = width;
			}

			inline void set_height(const Uint32 height)
			{
				m_rebuild = m_height != height;
				m_height = height;
			}

			inline void set_depth(const Uint32 depth)
			{
				m_rebuild = m_depth != depth;
				m_depth = depth;
			}

			inline void set_format(const TextureFormatType value)
			{
				m_rebuild = m_format != value;
				m_format = value;
			}

			inline void set_target(const TextureTargetType value)
			{
				m_rebuild = m_target != value;
				m_target = value;
			}

			inline void set_mag_filter(
				const TextureFilterType value)
			{
				m_mag_filter = value;
			}

			inline void set_min_filter(
				const TextureFilterType value)
			{
				m_min_filter = value;
			}

			inline void set_mipmap_filter(
				const TextureMipmapType value)
			{
				m_mipmap_filter = value;
			}

			inline void set_anisotropic_filter(const GLfloat value)
			{
				m_anisotropic_filter = value;
			}

			inline void set_mipmap_count(const Uint16 value)
			{
				m_mipmap_count = value;
			}

			inline void set_wrap_s(const TextureWrapType value)
			{
				m_wrap_s = value;
			}

			inline void set_wrap_t(const TextureWrapType value)
			{
				m_wrap_t = value;
			}

			inline void set_wrap_r(const TextureWrapType value)
			{
				m_wrap_r = value;
			}

			static inline void unbind()
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			void set_image(const ImageSharedPtr &image);
			void set_images(const Uint16 mipmaps,
				const ImageSharedPtrVector &images);
			void sub_texture(const Uint16 mipmap,
				const ImageSharedPtr &image,
				const Uint32Array3 &position);
			void sub_texture(const Uint16 texture_mipmap,
				const Uint16 image_mipmap,
				const ImageSharedPtr &image,
				const Uint32Array3 &texture_position,
				const Uint32Array3 &image_position,
				const Uint32Array3 &size);
			void init(const Uint32 width, const Uint32 height,
				const Uint32 depth, const Uint16 mipmaps);
			void attach_ext(const GLenum attachment,
				const Uint32 level, const Uint32 layer);
			void attach(const GLenum attachment, const Uint32 level,
				const Uint32 layer);
			void attach(const GLenum attachment,
				const CubeMapFaceType face, const Uint32 level);
			void attach(const GLenum attachment,
				const CubeMapFaceType face, const Uint32 level,
				const Uint32 layer);

			inline const String &get_name() const
			{
				return m_name;
			}

	};

	OutStream& operator<<(OutStream &str, const CubeMapFaceType value);
	OutStream& operator<<(OutStream &str, const TextureTargetType value);
	OutStream& operator<<(OutStream &str, const TextureFilterType value);
	OutStream& operator<<(OutStream &str, const TextureMipmapType value);
	OutStream& operator<<(OutStream &str, const TextureWrapType value);

}

#endif	/* UUID_f8a7405b_ced9_4d49_9ad0_cf51cc1a3dfd */
