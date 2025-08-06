#pragma once

#include "types.hpp"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define TILE_SIZE 16
#define COLOR_CHANNELS 4

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
		TextureAtlas();
		~TextureAtlas();

		void			init();
		typedef std::unordered_map<Material, std::vector<std::pair<BlockFace, std::string>>> TextureMap;
		void			loadTextures(const TextureMap & texturePaths);
		GLuint			getTextureID() const { return _id; }
		const 			BlockUV & getUVForBlock(Material material, BlockFace face) const;

		int				getTilesPerRow() const { return _tilesPerRow; }
		int				getWidth() const { return _atlasWidth; }
		int				getHeight() const { return _atlasHeight; }
	private:
		GLuint			_id;
		int				_atlasWidth;
		int				_atlasHeight;
		int				_tilesPerRow;
		typedef std::unordered_map<std::pair<Material, BlockFace>, BlockUV, UVMapHash> UVMap;
		UVMap			_uvMap;
		typedef std::unordered_map<std::string, glm::vec2> TextureCache;
		TextureCache	_textureCache;

		typedef std::vector<std::pair<BlockFace, std::string>> TextureBuffer;
		TextureBuffer	loadUnique(const std::string & texturePath);
		TextureBuffer	loadSideTop(const std::string & side, const std::string & topBottom);
		TextureBuffer	loadSideTopBottom(const std::string & side, const std::string & top, const std::string & bottom);

};
