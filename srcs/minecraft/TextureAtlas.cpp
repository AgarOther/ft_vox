#include "TextureAtlas.hpp"
#include "errors.hpp"
#include "utils.hpp"
#include <BlockTypeRegistry.hpp>
#include <cmath>
#include <cstddef>
#include <cstring>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

TextureAtlas::~TextureAtlas()
{
	if (_id)
		glDeleteTextures(1, &_id);
}

void TextureAtlas::load(const std::vector<std::string> & texturePaths)
{
	_tilesPerRow = static_cast<int>(ceil(sqrt(texturePaths.size())));
	_atlasWidth = _tilesPerRow * TILE_SIZE;
	_atlasHeight = _atlasWidth;

	std::vector<unsigned char> atlasData(_atlasWidth * _atlasHeight * COLOR_CHANNELS);

	for (size_t i = 0; i < texturePaths.size(); ++i)
	{
		int xIndex = i % _tilesPerRow;
		int yIndex = i / _tilesPerRow;
		int xOffset = xIndex * TILE_SIZE;
		int yOffset = yIndex * TILE_SIZE;
		int width, height, channels;

		stbi_uc * data = stbi_load(texturePaths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if (!data || width != TILE_SIZE || height != TILE_SIZE)
		{
			if (data)
				stbi_image_free(data);
			handleExit(5, FAILURE_ATLAS);
		}

		for (int row = 0; row < TILE_SIZE; ++row)
		{
			memcpy(&atlasData[((yOffset + row) * _atlasWidth + xOffset) * COLOR_CHANNELS],
					&data[row * TILE_SIZE * COLOR_CHANNELS],
					TILE_SIZE * COLOR_CHANNELS);
		}

		stbi_image_free(data);

		std::string name = texturePaths[i].substr(texturePaths[i].find_last_of("/\\") + 1);
		name = name.substr(0, name.find_last_of("."));
		Material material = BlockTypeRegistry::getBlockType(name).type;
		_uvMap[material] = {
			glm::vec2(
				static_cast<float>(xOffset) / _atlasWidth,
				static_cast<float>(yOffset) / _atlasHeight
			)
		};
	}

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _atlasWidth, _atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasData.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	#ifdef DEBUG
	stbi_write_png("atlas_debug.png", _atlasWidth, _atlasHeight, COLOR_CHANNELS, atlasData.data(), _atlasWidth * COLOR_CHANNELS);
	#endif
}

const BlockUV & TextureAtlas::getUVForBlock(Material material) const
{
	auto it = _uvMap.find(material);
	if (it == _uvMap.end())
		handleExit(6, FAILURE_ATLAS_UV);
	return it->second;
}
