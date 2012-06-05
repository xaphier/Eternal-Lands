#include "image.hpp"
#include "codec/codecmanager.hpp"
#include "abstractterrainmanager.hpp"
#include "readwritememory.hpp"
#include "reader.hpp"
#include "writer.hpp"

using namespace eternal_lands;

namespace
{

	glm::vec3 normals[256*256];

	void init_normals()
	{
		glm::uvec2 value;
		Uint32 i, j, index;

		for (i = 0; i < 256; ++i)
		{
			for (j = 0; j < 256; ++j)
			{
				value.x = j;
				value.y = i;

				index = value.x + value.y * 256;

				normals[index] =
					PackTool::decode_normal_optimized_uint8(
						value);
			}
		}
	}

	glm::uvec2 get_best_normal(const glm::vec3 &normal)
	{
		glm::uvec2 value, result;
		float error, tmp;
		Uint32 i, j, index;

		error = std::numeric_limits<float>::max();

		for (i = 0; i < 256; ++i)
		{
			for (j = 0; j < 256; ++j)
			{
				value.x = j;
				value.y = i;

				index = value.x + value.y * 256;

				tmp = std::abs(1.0f - glm::dot(normals[index],
					normal));

				if (tmp <= error)
				{
					error = tmp;
					result = value;
				}
			}
		}

		return result;
	}

	glm::vec3 get_position(const ImageSharedPtr &image, const Sint32 x,
		const Sint32 y)
	{
		glm::vec3 offset;
		glm::vec2 position;

		offset = glm::vec3(image->get_pixel(x, y, 0, 0, 0));
		offset *= AbstractTerrainManager::get_vector_scale();

		position.x = x;
		position.y = y;
		position *= AbstractTerrainManager::get_patch_scale();

		return glm::vec3(position, 0.0f) + offset;
	}

	glm::vec3 get_direction(const ImageSharedPtr &image,
		const glm::vec3 &centre, const Sint32 x, const Sint32 y)
	{
		if ((x < 0) || (y < 0) ||
			(x >= static_cast<Sint32>(image->get_width())) ||
			(y >= static_cast<Sint32>(image->get_height())))
		{
			return glm::vec3(0.0f);
		}

		return glm::normalize(get_position(image, x, y) - centre);
	}

}

void update_map_file(const std::string &file_name)
{
	std::fstream file;
	Uint8Array4 file_sig;
	Uint32 tile_x, tile_y, tile_map_offset, height_map_offset;
	Uint32 i, count;
	Uint8 tmp;

	file.open(file_name.c_str(), std::ios::in | std::ios::out |
		std::ios::binary);

	file.read(reinterpret_cast<char*>(file_sig.c_array()), 4);
	file.read(reinterpret_cast<char*>(&tile_x), 4);
	file.read(reinterpret_cast<char*>(&tile_y), 4);
	file.read(reinterpret_cast<char*>(&tile_map_offset), 4);
	file.read(reinterpret_cast<char*>(&height_map_offset), 4);

	file.seekp(tile_map_offset);

	tmp = 255;
	count = tile_x * tile_y;

	for (i = 0; i < count; ++i)
	{
		file.write(reinterpret_cast<char*>(&tmp), 1);
	}
}

int main(int argc, char *argv[])
{
	WriterSharedPtr writer;
	CodecManager codec_manager;
	ImageSharedPtr source_image, dest_image;
	ReaderSharedPtr reader;
	ReadWriteMemorySharedPtr buffer;
	glm::vec3 centre, d0, d1, d2, d3, d4, d5, d6, d7, n;
	glm::uvec3 sizes;
	glm::uvec2 value;
	Uint32 i, j, width, height, size;
	Sint32 x, y;
	std::ifstream str;

	if (argc < 3)
	{
		if (argc == 2)
		{
			update_map_file(argv[1]);
		}

		return -1;
	}

	init_normals();

	str.open(argv[1], std::ios::in | std::ios::binary);

	str.seekg(0, std::ios::end);
	size = str.tellg();
	str.seekg(0, std::ios::beg);

	buffer = boost::make_shared<ReadWriteMemory>(size);

	str.read(static_cast<char*>(buffer->get_ptr()), size);

	str.close();

	reader = boost::make_shared<Reader>(buffer, String(argv[1]));

	source_image = codec_manager.load_image(reader,
		ImageCompressionTypeSet(), true, false);

	sizes = source_image->get_sizes();

	sizes.x = ((sizes.x + 3) / 4) * 4;
	sizes.y = ((sizes.y + 3) / 4) * 4;
	sizes.z = 1;

	dest_image = boost::make_shared<Image>(String("terrain normals"),
		false, tft_rgb8, sizes, 0);

	width = source_image->get_width();
	height = source_image->get_height();

	for (j = 0; j < height; ++j)
	{
		for (i = 0; i < width; ++i)
		{
			x = i;
			y = j;

			centre = get_position(source_image, x, y);

			d0 = get_direction(source_image, centre, x - 1, y - 1);
			d1 = get_direction(source_image, centre, x + 0, y - 1);
			d2 = get_direction(source_image, centre, x + 1, y - 1);
			d3 = get_direction(source_image, centre, x + 1, y + 0);
			d4 = get_direction(source_image, centre, x + 1, y + 1);
			d5 = get_direction(source_image, centre, x + 0, y + 1);
			d6 = get_direction(source_image, centre, x - 1, y + 1);
			d7 = get_direction(source_image, centre, x - 1, y + 0);

			n = glm::cross(d0, d1);
			n += glm::cross(d1, d2);
			n += glm::cross(d2, d3);
			n += glm::cross(d3, d4);
			n += glm::cross(d4, d5);
			n += glm::cross(d5, d6);
			n += glm::cross(d6, d7);
			n += glm::cross(d7, d0);

			n = glm::normalize(n);

			value = PackTool::encode_normal_optimized_uint8(n);

			if (std::abs(1.0f - glm::dot(n,
				PackTool::decode_normal_optimized_uint8(value)))
				> 0.01f)
			{
				value = get_best_normal(n);
			}

			dest_image->set_pixel_uint(x, y, 0, 0, 0,
				glm::uvec4(value, 0, 0));
		}

		for (i = width; i < dest_image->get_width(); ++i)
		{
			x = i;
			y = j;

			dest_image->set_pixel_uint(x, y, 0, 0, 0,
				glm::uvec4(value, 0, 0));
		}
	}

	for (j = height; j < dest_image->get_height(); ++j)
	{
		for (i = 0; i < dest_image->get_width(); ++i)
		{
			x = i;
			y = j;

			dest_image->set_pixel_uint(x, y, 0, 0, 0,
				dest_image->get_pixel_uint(x, height - 1, 0, 0,
					0));
		}
	}

	writer = boost::make_shared<Writer>(String(argv[2]));

	codec_manager.save_image_as_png(dest_image, writer);

	return 1;
}

