/****************************************************************************
 *            terrainmaterialdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8ccca013_5e73_472f_9ec4_3b92bd4670b2
#define	UUID_8ccca013_5e73_472f_9ec4_3b92bd4670b2

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "exceptions.hpp"

/**
 * @file
 * @brief The @c class TerrainMaterialData.
 * This file contains the @c class TerrainMaterialData.
 */
namespace eternal_lands
{

	class TerrainMaterialData
	{
		private:
			Vec3Vector m_default_speculars;
			FloatVector m_default_glosses;
			FloatVector m_default_heights;
			FloatVector m_blend_sizes;
			BitSet64 m_use_blend_size_textures;
			BitSet64 m_use_specular_maps;
			BitSet64 m_use_gloss_maps;
			BitSet64 m_use_height_maps;
			bool m_write_height;
			bool m_write_specular_gloss;

			void load_layer_xml(const xmlNodePtr node,
				const Uint32 index);

		public:
			TerrainMaterialData();
			~TerrainMaterialData() noexcept;
			void load_xml(const xmlNodePtr node);
			void load_xml_string(const String &str);
			void save_xml(const XmlWriterSharedPtr &writer) const;
			String save_xml_string() const;

			inline Uint32 get_layer_count() const
			{
				return m_blend_sizes.size();
			}

			inline Uint16 get_static_height_index() const
			{
				Uint32 i, count;

				count = get_default_heights().size();

				assert(count > 0);

				for (i = 1; i < count; ++i)
				{
					if ((std::abs(get_default_height(0) -
						get_default_height(i)) >=
							epsilon) ||
						get_use_height_map(i))
						
					{
						return i - 1;
					}
				}

				return count;
			}

			inline Uint16 get_static_specular_index() const
			{
				Uint32 i, count;

				count = get_default_speculars().size();

				assert(count > 0);

				for (i = 1; i < count; ++i)
				{
					if (glm::any(glm::notEqualEpsilon(
						get_default_specular(0),
						get_default_specular(i),
						epsilon)) ||
						get_use_specular_map(i))
					{
						return i - 1;
					}
				}

				return count;
			}

			inline Uint16 get_static_gloss_index() const
			{
				Uint32 i, count;

				count = get_default_glosses().size();

				assert(count > 0);

				for (i = 1; i < count; ++i)
				{
					if ((std::abs(get_default_gloss(0) -
						get_default_gloss(i)) >=
							epsilon) ||
						get_use_gloss_map(i))
						
					{
						return i - 1;
					}
				}

				return count;
			}

			inline bool get_has_fixed_height() const
			{
				Uint32 i, count;

				count = get_default_heights().size();

				assert(count > 0);

				if (get_use_any_height_map())
				{
					return false;
				}

				for (i = 1; i < count; ++i)
				{
					if (std::abs(get_default_height(0) -
						get_default_height(i)) >=
							epsilon)
					{
						return false;
					}
				}

				return true;
			}

			inline bool get_has_fixed_specular() const
			{
				Uint32 i, count;

				count = get_default_speculars().size();

				assert(count > 0);

				if (get_use_any_specular_map())
				{
					return false;
				}

				for (i = 1; i < count; ++i)
				{
					if (glm::any(glm::notEqualEpsilon(
						get_default_specular(0),
						get_default_specular(i),
						epsilon)))
					{
						return false;
					}
				}

				return true;
			}

			inline bool get_has_fixed_gloss() const
			{
				Uint32 i, count;

				count = get_default_glosses().size();

				assert(count > 0);

				if (get_use_any_gloss_map())
				{
					return false;
				}

				for (i = 1; i < count; ++i)
				{
					if (std::abs(get_default_gloss(0) -
						get_default_gloss(i)) >=
							epsilon)
					{
						return false;
					}
				}

				return true;
			}

			inline bool get_use_any_blend_size_texture() const
			{
				BitSet64 mask;

				mask = (1 << m_blend_sizes.size()) - 1;

				return (m_use_blend_size_textures & mask).any();
			}

			inline bool get_use_any_none_blend_size_texture() const
			{
				BitSet64 mask;

				mask = (1 << m_blend_sizes.size()) - 1;

				return ((~m_use_blend_size_textures) &
					mask).any();
			}

			inline bool get_use_any_height_map() const
			{
				BitSet64 mask;

				mask = (1 << m_blend_sizes.size()) - 1;

				return (m_use_height_maps & mask).any();
			}

			inline bool get_use_any_specular_map() const
			{
				BitSet64 mask;

				mask = (1 << m_blend_sizes.size()) - 1;

				return (m_use_specular_maps & mask).any();
			}

			inline bool get_use_any_gloss_map() const
			{
				BitSet64 mask;

				mask = (1 << m_blend_sizes.size()) - 1;

				return (m_use_gloss_maps & mask).any();
			}

			inline void resize(const Uint16 size)
			{
				Uint32 i, count;

				RANGE_CECK_MAX(size,
					m_use_blend_size_textures.size(),
					UTF8("size too big."));

				count = m_use_blend_size_textures.size();

				for (i = size; i < count; ++i)
				{
					m_use_blend_size_textures[i] = false;
					m_use_specular_maps[i] = false;
					m_use_gloss_maps[i] = false;
					m_use_height_maps[i] = false;
				}

				m_default_speculars.resize(size,
					glm::vec3(0.0f));
				m_default_glosses.resize(size, 0.0f);
				m_default_heights.resize(size, 0.0f);
				m_blend_sizes.resize(size, 0.1f);
			}

			inline void clear()
			{
				m_default_speculars.clear();
				m_default_glosses.clear();
				m_default_heights.clear();
				m_blend_sizes.clear();
				m_use_blend_size_textures.reset();
				m_use_specular_maps.reset();
				m_use_gloss_maps.reset();
				m_use_height_maps.reset();
				m_write_height = true;
				m_write_specular_gloss = true;
			}

			inline float get_blend_size(const Uint16 index) const
			{
				RANGE_CECK_MAX(index, m_blend_sizes.size() - 1,
					UTF8("index too big."));

				return m_blend_sizes[index];
			}

			inline void set_blend_size(const float blend_size,
				const Uint16 index)
			{
				RANGE_CECK_MAX(index, m_blend_sizes.size() - 1,
					UTF8("index too big."));

				m_blend_sizes[index] = blend_size;
			}

			inline const FloatVector &get_blend_sizes() const
			{
				return m_blend_sizes;
			}

			inline void set_blend_sizes(
				const FloatVector &blend_sizes)
			{
				m_blend_sizes = blend_sizes;
			}

			inline bool get_use_height_map(const Uint16 index) const
			{
				RANGE_CECK_MAX(index,
					m_use_height_maps.size() - 1,
					UTF8("index too big."));

				return m_use_height_maps[index];
			}

			inline bool get_use_specular_map(const Uint16 index)
				const
			{
				RANGE_CECK_MAX(index,
					m_use_specular_maps.size() - 1,
					UTF8("index too big."));

				return m_use_specular_maps[index];
			}

			inline bool get_use_gloss_map(const Uint16 index) const
			{
				RANGE_CECK_MAX(index,
					m_use_gloss_maps.size() - 1,
					UTF8("index too big."));

				return m_use_gloss_maps[index];
			}

			inline bool get_use_blend_size_texture(
				const Uint16 index) const
			{
				RANGE_CECK_MAX(index,
					m_use_blend_size_textures.size() - 1,
					UTF8("index too big."));

				return m_use_blend_size_textures[index];
			}

			inline const BitSet64 &get_use_height_maps() const
			{
				return m_use_height_maps;
			}

			inline const BitSet64 &get_use_specular_maps() const
			{
				return m_use_specular_maps;
			}

			inline const BitSet64 &get_use_gloss_maps() const
			{
				return m_use_gloss_maps;
			}

			inline const BitSet64 &get_use_blend_size_textures()
				const
			{
				return m_use_blend_size_textures;
			}

			inline float get_default_height(const Uint16 index)
				const
			{
				RANGE_CECK_MAX(index, m_default_heights.size(),
					UTF8("index too big."));

				return m_default_heights[index];
			}

			inline const glm::vec3 &get_default_specular(
				const Uint16 index) const
			{
				RANGE_CECK_MAX(index,
					m_default_speculars.size() - 1,
					UTF8("index too big."));

				return m_default_speculars[index];
			}

			inline float get_default_gloss(const Uint16 index) const
			{
				RANGE_CECK_MAX(index,
					m_default_glosses.size() - 1,
					UTF8("index too big."));

				return m_default_glosses[index];
			}

			inline const FloatVector &get_default_heights() const
			{
				return m_default_heights;
			}

			inline const Vec3Vector &get_default_speculars() const
			{
				return m_default_speculars;
			}

			inline const FloatVector &get_default_glosses() const
			{
				return m_default_glosses;
			}

			inline bool get_write_height() const noexcept
			{
				return m_write_height;
			}

			inline bool get_write_specular_gloss() const noexcept
			{
				return m_write_specular_gloss;
			}

			inline void set_use_height_map(
				const bool use_height_map, const Uint16 index)
			{
				RANGE_CECK_MAX(index,
					m_use_height_maps.size() - 1,
					UTF8("index too big."));

				m_use_height_maps[index] = use_height_map;
			}

			inline void set_use_specular_map(
				const bool use_specular_map,
				const Uint16 index)
			{
				RANGE_CECK_MAX(index,
					m_use_specular_maps.size() - 1,
					UTF8("index too big."));

				m_use_specular_maps[index] = use_specular_map;
			}

			inline void set_use_gloss_map(const bool use_gloss_map,
				const Uint16 index)
			{
				RANGE_CECK_MAX(index,
					m_use_gloss_maps.size() - 1,
					UTF8("index too big."));

				m_use_gloss_maps[index] = use_gloss_map;
			}

			inline void set_use_blend_size_texture(
				const bool use_blend_size_texture,
				const Uint16 index)
			{
				RANGE_CECK_MAX(index,
					m_use_blend_size_textures.size() - 1,
					UTF8("index too big."));

				m_use_blend_size_textures[index] =
					use_blend_size_texture;
			}

			inline void set_use_height_maps(
				const BitSet64 &use_height_maps)
			{
				m_use_height_maps = use_height_maps;
			}

			inline void set_use_specular_maps(
				const BitSet64 &use_specular_maps)
			{
				m_use_specular_maps = use_specular_maps;
			}

			inline void set_use_gloss_maps(
				const BitSet64 &use_gloss_maps)
			{
				m_use_gloss_maps = use_gloss_maps;
			}

			inline void set_use_blend_size_textures(
				const BitSet64 &use_blend_size_textures)
			{
				m_use_blend_size_textures =
					use_blend_size_textures;
			}

			inline void set_default_height(
				const float default_height, const Uint16 index)
			{
				RANGE_CECK_MAX(index,
					m_use_height_maps.size() - 1,
					UTF8("index too big."));

				m_default_heights[index] = default_height;
			}

			inline void set_default_specular(
				const glm::vec3 &default_specular,
				const Uint16 index)
			{
				RANGE_CECK_MAX(index,
					m_default_speculars.size() - 1,
					UTF8("index too big."));

				m_default_speculars[index] = default_specular;
			}

			inline void set_default_gloss(
				const float default_gloss, const Uint16 index)
			{
				RANGE_CECK_MAX(index,
					m_default_glosses.size() - 1,
					UTF8("index too big."));

				m_default_glosses[index] = default_gloss;
			}

			inline void set_default_heights(
				const FloatVector &default_heights)
			{
				m_default_heights = default_heights;
			}

			inline void set_default_speculars(
				const Vec3Vector &default_speculars)
			{
				m_default_speculars = default_speculars;
			}

			inline void set_default_glosses(
				const FloatVector &default_glosses)
			{
				m_default_glosses = default_glosses;
			}

			inline void set_write_height(const bool write_height)
				noexcept
			{
				m_write_height = write_height;
			}

			inline void set_write_specular_gloss(
				const bool write_specular_gloss) noexcept
			{
				m_write_specular_gloss = write_specular_gloss;
			}

	};

}

#endif	/* UUID_bdba47be_bf5a_4315_b031_ea6027e40175 */
