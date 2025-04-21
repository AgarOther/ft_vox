/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:38:11 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 02:46:32 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shader.hpp"
#include "Utils.hpp"

Shader *currentlyBound;

Shader &Shader::getCurrentlyBoundShader()
{
	return (*currentlyBound);
}

Shader::Shader(const char *vertFile, const char *fragFile)
{
	std::string vertSrc = Utils::getShaderAsString(vertFile);
	std::string fragSrc = Utils::getShaderAsString(fragFile);
	this->_id = Utils::compileShader(vertSrc.c_str(), fragSrc.c_str());
	if (this->_id != 0)
		currentlyBound = this;
}

GLuint Shader::getId() const
{
	return (this->_id);
}

void Shader::use()
{
	glUseProgram(this->_id);
}

void Shader::free()
{
	glDeleteProgram(this->_id);
}

void Shader::setMat4(const char *uniform, glm::mat4 model)
{
	GLuint id = glGetUniformLocation(this->_id, uniform);
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::setVec4(const char *uniform, glm::vec4 vec)
{
	GLuint id = glGetUniformLocation(this->_id, uniform);
	glUniform4fv(id, 1, glm::value_ptr(vec));
}

void Shader::setInt(const char *uniform, int n)
{
	GLuint id = glGetUniformLocation(this->_id, uniform);
	glUniform1i(id, n);
}

void Shader::setTint(Material material)
{
	this->setInt("tintFace", 0);
	this->setVec4("tintCol", glm::vec4(1.0f));
	if (material == Material::GRASS_BLOCK)
	{
		this->setInt("tintFace", 5);
		this->setVec4("tintCol", glm::vec4(0.498f, 0.698f, 0.219f, 1.0f));
	}
}
