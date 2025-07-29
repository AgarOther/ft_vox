#include "Texture.hpp"
#include <GL/gl.h>
#include <stb/stb_image.h>

Texture::Texture(const std::string & filepath, const GLenum textureType, const GLenum slot, const GLenum format, const GLenum pixeltype): _type(textureType)
{
	int width, height, colorChannels;
	unsigned char *bytes;

	stbi_set_flip_vertically_on_load(true);
	bytes = stbi_load(filepath.c_str(), &width, &height, &colorChannels, 3);
	glGenTextures(1, &_id);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(textureType, _id);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(textureType, 0, GL_RGB, width, height, 0, format, pixeltype, bytes);
	glGenerateMipmap(textureType);
	stbi_image_free(bytes);
	glBindTexture(textureType, 0);
}

void Texture::bind() const
{
	glBindTexture(_type, _id);
}

void Texture::unbind() const
{
	glBindTexture(_type, 0);
}
