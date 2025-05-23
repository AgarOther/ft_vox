/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:20:08 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 02:47:05 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TextureType.hpp"
#include "Shader.hpp"
#include <stb/stb_image.h>
#include <string>

static std::string getFilePath(const char *image)
{
	std::string path = "resources/textures/block/";
	path.append(image);
	return (path);
}

Texture::Texture(const char *image, const GLenum textureType, const GLenum slot, const GLenum format, const GLenum pixeltype)
{
	this->_type = textureType;
	// Loading image
	int width, height, colorChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *bytes = stbi_load(getFilePath(image).c_str(), &width, &height, &colorChannels, 3);
	
	glGenTextures(1, &this->_id);
	// Set the first texture slot
	glActiveTexture(GL_TEXTURE0 + slot);
	// Bind a texture to the set slot
	glBindTexture(textureType, this->_id);

	// Downscale (MIN) & Upscale (MAG) resize option
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(textureType, 0, GL_RGB, width, height, 0, format, pixeltype, bytes);
	glGenerateMipmap(textureType);

	stbi_image_free(bytes);
	glBindTexture(textureType, 0);
}

GLuint Texture::getId() const
{
	return (this->_id);
}

GLenum Texture::getType() const
{
	return (this->_type);
}

void Texture::bind() const
{
	glBindTexture(this->_type, this->_id);
}

void Texture::unbind() const
{
	glBindTexture(this->_type, 0);
}

void Texture::free() const
{
	glDeleteTextures(1, &this->_id);
}

void Texture::resetSlots()
{
	for (int i = 0; i < 6; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
