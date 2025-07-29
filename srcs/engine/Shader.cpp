#include <iostream>
#include "Shader.hpp"
#include "utils.hpp"
#include "colors.hpp"

static GLuint createShader(GLenum type, const std::string & filepath)
{
	GLuint shader;
	int result;
	std::string shaderSrc;
	char * vertexStr;

	shader = glCreateShader(type);
	shaderSrc = getShaderAsString(filepath);
	vertexStr = (char *)shaderSrc.c_str();
	glShaderSource(shader, 1, &vertexStr, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int error_length;
		char *error_message;
		// Get the log message's length in order to alloca the string to store it.
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_length);
		// Alloca is a malloc that frees itself once the function ends.
		error_message = (char *)alloca(sizeof(char) * error_length);
		// This will store the log of the program into the error_message string.
		glGetShaderInfoLog(shader, error_length, &error_length, error_message);
		std::cout << BOLD_RED << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "shader failed to compile." << std::endl;
		std::cout << error_message << RESET << std::endl;
		// Delete the shader to avoid leaks.
		glDeleteShader(shader);
		return (0);
	}
	return shader;
}

Shader::Shader(const std::string & vertexPath, const std::string & fragmentPath)
{
	GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexPath);
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentPath);
	_id = glCreateProgram();
	glAttachShader(_id, vertexShader);
	glAttachShader(_id, fragmentShader);
	glLinkProgram(_id);
	glValidateProgram(_id);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::setMat4(const char *uniform, const glm::mat4 &model) const
{
	const GLint id = glGetUniformLocation(this->_id, uniform);
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(model));
}

Shader::~Shader()
{
	glDeleteProgram(_id);
}

void Shader::bind() const
{
	glUseProgram(_id);
}

void Shader::unbind()
{
	glUseProgram(0);
}
