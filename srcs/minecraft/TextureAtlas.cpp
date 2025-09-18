#include "TextureAtlas.hpp"
#include "errors.hpp"
#include "utils.hpp"
#include <BlockTypeRegistry.hpp>
#include <cmath>
#include <cstring>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "colors.hpp"
#include <iostream>
#include <unordered_map>

TextureAtlas::TextureAtlas(): _id(0), _atlasWidth(0), _atlasHeight(0)
{
	
}

TextureAtlas::~TextureAtlas()
{
	if (_id)
		glDeleteTextures(1, &_id);
}

void TextureAtlas::loadTextures(const std::unordered_map<Material, std::vector<std::pair<BlockFace, std::string>>> & texturePaths)
{
	static const std::string prefix = "assets/block/";
	static int texCount = 0;

	_tilesPerRow = ceil(sqrt(texturePaths.size())) + 1;
	_atlasWidth = _tilesPerRow * TILE_SIZE;
	_atlasHeight = _atlasWidth;

	std::vector<unsigned char> atlasData(_atlasWidth * _atlasHeight * COLOR_CHANNELS);

	stbi_set_flip_vertically_on_load(true);
	for (size_t material = UNKNOWN; material < texturePaths.size(); material++)
	{
		auto it = texturePaths.find(static_cast<Material>(material));
		if (it == texturePaths.end())
			handleExit(FAILURE_ATLAS_LOAD);
		std::vector<std::pair<BlockFace, std::string>> textureContainer = it->second;
		if (textureContainer.size() != 6)
		{
			std::cout << YELLOW << "Warning: Sent texturePaths of wrong size." << RESET << std::endl;
			return;
		}
		for (int face = FACE_FRONT; face <= FACE_BOTTOM; ++face)
		{
			int xIndex = texCount % _tilesPerRow;
			int yIndex = texCount / _tilesPerRow;
			int xOffset = xIndex * TILE_SIZE;
			int yOffset = yIndex * TILE_SIZE;
			int width, height, channels;

			BlockFace textureFace = textureContainer[face].first;
			std::string texturePath = textureContainer[face].second;
			if (_textureCache.find(texturePath) != _textureCache.end())
			{
				_uvMap[std::pair<Material, BlockFace>(static_cast<Material>(material), textureFace)]= _textureCache[texturePath];
				continue;
			}
			stbi_uc * data = stbi_load((prefix + texturePath).c_str(), &width, &height, &channels, STBI_rgb_alpha);
			if (!data || width != TILE_SIZE || height != TILE_SIZE)
			{
				if (data)
					stbi_image_free(data);
				handleExit(FAILURE_ATLAS);
			}

			for (int row = 0; row < TILE_SIZE; ++row)
			{
				memcpy(&atlasData[((yOffset + row) * _atlasWidth + xOffset) * COLOR_CHANNELS],
						&data[row * TILE_SIZE * COLOR_CHANNELS],
						TILE_SIZE * COLOR_CHANNELS);
			}

			stbi_image_free(data);

			const glm::vec2 uv = glm::vec2(
				static_cast<float>(xOffset) / _atlasWidth,
				static_cast<float>(yOffset) / _atlasHeight
			);

			_textureCache[texturePath] = uv;
			_uvMap[std::pair<Material, BlockFace>(static_cast<Material>(material), textureFace)] = uv;
			texCount++;
		}
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

const BlockUV & TextureAtlas::getUVForBlock(Material material, BlockFace face) const
{
	auto it = _uvMap.find(std::pair<Material, BlockFace>(material, face));
	if (it == _uvMap.end())
		handleExit(FAILURE_ATLAS_UV);
	return it->second;
}

void TextureAtlas::init()
{
	TextureMap textureMap;

	// Unique textures (one texture for the whole block)
	textureMap[UNKNOWN]				= loadUnique("unknown.png");
	textureMap[STONE]				= loadUnique("stone.png");
	textureMap[END_STONE]			= loadUnique("end_stone.png");
	textureMap[BEDROCK]				= loadUnique("bedrock.png");
	textureMap[DIRT]				= loadUnique("dirt.png");
	textureMap[SAND]				= loadUnique("sand.png");
	textureMap[OAK_LEAVES]			= loadUnique("oak_leaves.png");
	textureMap[WATER]				= loadUnique("water_full.png");
	textureMap[LAVA]				= loadUnique("lava_still.png");
	textureMap[NETHERRACK]			= loadUnique("netherrack.png");
	textureMap[GLOWSTONE]			= loadUnique("glowstone.png");
	textureMap[GRAVEL]				= loadUnique("gravel.png");
	textureMap[SOUL_SAND]			= loadUnique("soul_sand.png");
	textureMap[CHERRY_LEAVES]		= loadUnique("cherry_leaves.png");
	textureMap[OBSIDIAN]			= loadUnique("obsidian.png");

	// Multi-textured blocks
	textureMap[OAK_LOG]				= loadSideTop("oak_log.png", "oak_log_top.png");
	textureMap[ACACIA_LOG]			= loadSideTop("acacia_log.png", "acacia_log_top.png");
	textureMap[CHERRY_LOG]			= loadSideTop("cherry_log.png", "cherry_log_top.png");

	TextureBuffer cartography;
	cartography.push_back(std::pair<BlockFace, std::string>((FACE_FRONT), "cartography_table_side1.png"));
	cartography.push_back(std::pair<BlockFace, std::string>((FACE_BACK), "cartography_table_side3.png"));
	cartography.push_back(std::pair<BlockFace, std::string>((FACE_LEFT), "cartography_table_side2.png"));
	cartography.push_back(std::pair<BlockFace, std::string>((FACE_RIGHT), "cartography_table_side3.png"));
	cartography.push_back(std::pair<BlockFace, std::string>((FACE_TOP), "cartography_table_top.png"));
	cartography.push_back(std::pair<BlockFace, std::string>((FACE_BOTTOM), "dark_oak_planks.png"));
	textureMap[CARTOGRAPHY_TABLE]	= cartography;

	textureMap[GRASS_BLOCK]			= loadSideTopBottom("grass_block_side.png", "grass_block_top.png", "dirt.png");
	loadTextures(textureMap);
}
