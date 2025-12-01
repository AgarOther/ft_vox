#pragma once

#include "glm/glm.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "BlockTypeRegistry.hpp"

#define TILE_SIZE 16
#define COLOR_CHANNELS 4

typedef std::vector<std::pair<BlockFace, std::string>> TextureBuffer;
typedef glm::vec2 BlockUV;

struct UVMapHash
{
	size_t operator()(const std::pair<Material, BlockFace> & uv) const
	{
		return std::hash<int>()(uv.first) ^ (std::hash<int>()(uv.second) << 1);
	}
};

class TextureAtlas
{
	public:
		TextureAtlas() = delete;
		~TextureAtlas() = delete;

		static void				init();
		static void				destroy();

		static GLuint			getTextureID() { return _id; }
		static const BlockUV &	getUVForBlock(Material material, BlockFace face);
		static int				getTilesPerRow() { return _tilesPerRow; }
		static int				getWidth() { return _atlasWidth; }
		static int				getHeight() { return _atlasHeight; }

	private:
		typedef std::unordered_map<Material, TextureBuffer> TextureMap;
		typedef std::unordered_map<std::pair<Material, BlockFace>, BlockUV, UVMapHash> UVMap;
		typedef std::unordered_map<std::string, glm::vec2> TextureCache;

		static void				_loadTextures(const TextureMap & texturePaths);

		static GLuint			_id;
		static int				_atlasWidth;
		static int				_atlasHeight;
		static int				_tilesPerRow;
		static UVMap			_uvMap;
		static TextureCache		_textureCache;
};
