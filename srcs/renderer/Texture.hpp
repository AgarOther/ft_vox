#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture
{
	public:
		Texture(const std::string & filepath, const GLenum textureType, const GLenum slot, const GLenum format, const GLenum pixeltype);
		~Texture();

		void bind() const;
		void unbind() const;

	private:
		GLuint	_id;
		GLenum _type;
};
