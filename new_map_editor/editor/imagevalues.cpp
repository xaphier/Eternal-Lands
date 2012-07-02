/****************************************************************************
 *            imagevalues.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "imagevalues.hpp"
#include "readwritememory.hpp"
#include "lz4.hpp"

namespace eternal_lands
{

	ImageValues::ImageValues(): m_size(0)
	{
	}

	ImageValues::~ImageValues() noexcept
	{
	}

	void ImageValues::pack(const ImageValueVector &image_values)
	{
		ReadWriteMemorySharedPtr buffer;
		Uint32 x, y, i, j, count, max_count, idx0, idx1, image_count;
		Uint16* data;

		buffer = boost::make_shared<ReadWriteMemory>(
			image_values.size() *
				(ImageValue::get_image_count() + 3) *
				sizeof(Uint16));

		data = static_cast<Uint16*>(buffer->get_ptr());

		idx0 = 0;
		idx1 = 0;

		image_count = ImageValue::get_image_count();

		while (idx1 < image_values.size())
		{
			count = 1;
			max_count = std::min(static_cast<Uint32>(
					image_values.size() - idx1),
				static_cast<Uint32>(
					std::numeric_limits<Uint16>::max()
					+ 1));
			x = image_values[idx1].get_x();
			y = image_values[idx1].get_y();

			while (count < max_count)
			{
				if ((image_values[idx1 + count].get_x() != x) ||
					(image_values[idx1 + count].get_y() !=
						(y + count)))
				{
					break;
				}

				++count;
			}

			data[idx0 + 0] = x;
			data[idx0 + 1] = y;
			data[idx0 + 2] = count - 1;
			idx0 += 3;

			for (i = 0; i < count; ++i)
			{
				for (j = 0; j < image_count; ++j)
				{
					data[idx0] = image_values[
						idx1].get_packed_value(j);
					idx0++;
				}
				idx1++;
			}
		}

		m_buffer = Lz4::compress_high(buffer);
		m_size = buffer->get_size();
	}

	ImageValueVector ImageValues::unpack() const
	{
		ImageValueVector image_values;
		ReadWriteMemorySharedPtr buffer;
		Uint32 x, y, i, j, count, idx0, idx1, image_count;
		Uint32 size;
		Uint16* data;

		buffer = Lz4::decompress(m_buffer, m_size);

		data = static_cast<Uint16*>(buffer->get_ptr());

		size = buffer->get_size() / sizeof(Uint16);

		if (size <= 4)
		{
			return image_values;
		}

		idx0 = 0;
		idx1 = 0;

		image_count = ImageValue::get_image_count();

		while (true)
		{
			if ((idx0 + 3) >= size)
			{
				return image_values;
			}

			x = data[idx0 + 0];
			y = data[idx0 + 1];
			count = static_cast<Uint32>(data[idx0 + 2]) + 1;

			idx0 += 3;

			if ((idx0 + 3) >= count)
			{
				return image_values;
			}

			for (i = 0; i < count; ++i)
			{
				image_values.push_back(ImageValue(x, y));

				for (j = 0; j < image_count; ++j)
				{
					image_values[idx1].set_packed_value(
						data[idx0], j);
					idx0++;
				}

				y++;
				idx1++;
			}
		}

		return image_values;
	}

}
