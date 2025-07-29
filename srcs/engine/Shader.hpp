#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Shader
{
	public:
		Shader(const std::string & vertexShader, const std::string & fragmentShader);
		~Shader();

		GLuint		getId() const { return _id; }
		void		setMat4(const char * uniform, const glm::mat4 & model) const;
		void		bind() const;
		static void unbind();
	private:
		GLuint		_id;	
};
