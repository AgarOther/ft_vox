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

class TextureAtlas
{
	public:
		TextureAtlas(): _id(0), _atlasWidth(0), _atlasHeight(0) {}
		~TextureAtlas();

		void	load(const std::vector<std::string> & texturePaths);
		GLuint	getTextureID() const { return _id; }
		const 	BlockUV & getUVForBlock(Material material) const;

		int		getTilesPerRow() const { return _tilesPerRow; }
		int		getWidth() const { return _atlasWidth; }
		int		getHeight() const { return _atlasHeight; }
	private:
		GLuint	_id;
		int		_atlasWidth;
		int		_atlasHeight;
		int		_tilesPerRow;
		typedef std::unordered_map<Material, BlockUV> UVMap;
		UVMap	_uvMap;
};
