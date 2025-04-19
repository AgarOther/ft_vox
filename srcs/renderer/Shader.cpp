/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:38:11 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 15:25:53 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shader.hpp"
#include "Utils.hpp"

Shader::Shader(const char *vertFile, const char *fragFile)
{
	std::string vertSrc = Utils::getShaderAsString(vertFile);
	std::string fragSrc = Utils::getShaderAsString(fragFile);
	this->_id = Utils::compileShader(vertSrc.c_str(), fragSrc.c_str());
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

void Shader::setInt(const char *uniform, int n)
{
	GLuint id = glGetUniformLocation(this->_id, uniform);
	glUniform1i(id, n);
}
