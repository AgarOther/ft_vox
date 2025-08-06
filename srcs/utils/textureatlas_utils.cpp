#include "TextureAtlas.hpp"

TextureBuffer loadUnique(const std::string & texturePath)
{
	TextureBuffer paths;

	for (int face = FACE_FRONT; face <= FACE_BOTTOM; ++face)
		paths.push_back(std::pair<BlockFace, std::string>(static_cast<BlockFace>(face), texturePath));
	return paths;
}

TextureBuffer loadSideTop(const std::string & side, const std::string & topBottom)
{
	TextureBuffer paths;

	for (int face = FACE_FRONT; face <= FACE_BOTTOM; ++face)
	{
		if (face == FACE_TOP || face == FACE_BOTTOM)
			paths.push_back(std::pair<BlockFace, std::string>(static_cast<BlockFace>(face), topBottom));
		else
			paths.push_back(std::pair<BlockFace, std::string>(static_cast<BlockFace>(face), side));
	}
	return paths;
}

TextureBuffer loadSideTopBottom(const std::string & side, const std::string & top, const std::string & bottom)
{
	TextureBuffer paths;

	for (int face = FACE_FRONT; face <= FACE_BOTTOM; ++face)
	{
		if (face == FACE_TOP)
			paths.push_back(std::pair<BlockFace, std::string>(static_cast<BlockFace>(face), top));
		else if (face == FACE_BOTTOM)
			paths.push_back(std::pair<BlockFace, std::string>(static_cast<BlockFace>(face), bottom));
		else
			paths.push_back(std::pair<BlockFace, std::string>(static_cast<BlockFace>(face), side));
	}
	return paths;
}