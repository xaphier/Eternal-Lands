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
#include "blenddata.hpp"
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
			BlendDataVector m_blend_datas;
			BitSet64 m_use_blend_size_samplers;
			BitSet64 m_use_extra_maps;

		public:
			TerrainMaterialData();
			~TerrainMaterialData() noexcept;

			inline bool get_use_any_blend_size_sampler() const
			{
				BitSet64 mask;

				mask = (1 << m_blend_datas.size()) - 1;

				return (m_use_blend_size_samplers & mask).any();
			}

			inline bool get_use_any_none_blend_size_sampler() const
			{
				BitSet64 mask;

				mask = (1 << m_blend_datas.size()) - 1;

				return ((~m_use_blend_size_samplers) &
					mask).any();
			}

			inline bool get_use_any_extra_map() const
			{
				BitSet64 mask;

				mask = (1 << m_blend_datas.size()) - 1;

				return (m_use_extra_maps & mask).any();
			}

			inline Uint32 get_blend_datas_size() const
			{
				return m_blend_datas.size();
			}

			inline void resize(const Uint16 size)
			{
				RANGE_CECK_MAX(size,
					m_use_extra_maps.size() - 1,
					UTF8("size too big."));

				m_blend_datas.resize(size);
			}

			inline void clear()
			{
				m_blend_datas.clear();
			}

			inline const BlendData &get_blend_data(
				const Uint16 index) const
			{
				RANGE_CECK_MAX(index, m_blend_datas.size(),
					UTF8("index too big."));

				return m_blend_datas[index];
			}

			inline void set_blend_data(const BlendData &blend_data,
				const Uint16 index)
			{
				RANGE_CECK_MAX(index, m_blend_datas.size(),
					UTF8("index too big."));

				m_blend_datas[index] = blend_data;
			}

			inline void append_blend_data(
				const BlendData &blend_data)
			{
				RANGE_CECK_MAX(m_blend_datas.size() + 1,
					m_use_extra_maps.size() - 1,
					UTF8("index too big."));

				m_blend_datas.push_back(blend_data);
			}

			inline const BlendDataVector &get_blend_datas() const
			{
				return m_blend_datas;
			}

			inline void set_blend_datas(
				const BlendDataVector &blend_datas)
			{
				RANGE_CECK_MAX(blend_datas.size(),
					m_use_extra_maps.size() - 1,
					UTF8("size too big."));

				m_blend_datas = blend_datas;
			}

			inline bool get_use_extra_map(const Uint16 index) const
			{
				RANGE_CECK_MAX(index,
					m_use_extra_maps.size(),
					UTF8("index too big."));

				return m_use_extra_maps[index];
			}

			inline const BitSet64 &get_use_extra_maps() const
			{
				return m_use_extra_maps;
			}

			inline void set_use_extra_map(const bool use_extra_map,
				const Uint16 index)
			{
				RANGE_CECK_MAX(index, m_use_extra_maps.size(),
					UTF8("index too big."));

				m_use_extra_maps[index] = use_extra_map;
			}

			inline void set_use_extra_maps(
				const BitSet64 &use_extra_maps)
			{
				m_use_extra_maps = use_extra_maps;
			}

			inline bool get_use_blend_size_sampler(
				const Uint16 index) const
			{
				RANGE_CECK_MAX(index,
					m_use_blend_size_samplers.size(),
					UTF8("index too big."));

				return m_use_blend_size_samplers[index];
			}

			inline const BitSet64 &get_use_blend_size_samplers()
				const
			{
				return m_use_blend_size_samplers;
			}

			inline void set_use_blend_size_sampler(
				const bool use_blend_size_sampler,
				const Uint16 index)
			{
				RANGE_CECK_MAX(index,
					m_use_blend_size_samplers.size(),
					UTF8("index too big."));

				m_use_blend_size_samplers[index] =
					use_blend_size_sampler;
			}

			inline void set_use_blend_size_samplers(
				const BitSet64 &use_blend_size_samplers)
			{
				m_use_blend_size_samplers =
					use_blend_size_samplers;
			}

	};

}

#endif	/* UUID_bdba47be_bf5a_4315_b031_ea6027e40175 */
