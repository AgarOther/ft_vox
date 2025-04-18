/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:38:11 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/18 23:36:15 by scraeyme         ###   ########.fr       */
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
