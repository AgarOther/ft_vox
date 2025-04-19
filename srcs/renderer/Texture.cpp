/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:20:08 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 15:36:45 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Texture.hpp"
#include "TextureType.hpp"
#include <stb/stb_image.h>

Texture::Texture()
{
	
}

Texture::Texture(const char *image, GLenum textureType, GLenum slot, GLenum format, GLenum pixeltype)
{
	this->_type = textureType;
	// Loading image
	int width, height, colorChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *bytes = stbi_load(image, &width, &height, &colorChannels, 0);
	
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

	glTexImage2D(textureType, 0, GL_RGBA, width, height, 0, format, pixeltype, bytes);
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

void Texture::bind()
{
	glBindTexture(this->_type, this->_id);
}

void Texture::unbind()
{
	glBindTexture(this->_type, 0);
}

void Texture::free()
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



