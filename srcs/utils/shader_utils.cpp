/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:56:35 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 02:14:25 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "colors.hpp"
#include <iostream>

static bool didShaderCompile(GLuint id, int type)
{
	int				result;
	int				error_length;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &error_length);
		char *error_message = static_cast<char *>(alloca(error_length * sizeof(char)));
		glGetShaderInfoLog(id, error_length, &error_length, error_message);
		std::cout << BOLD_RED << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader failed to compile:" << std::endl;
		std::cout << error_message << RESET << std::endl;
		glDeleteShader(id);
		return (false);
	}
	return (true);
}

GLuint Utils::compileShader(const char *vertSrc, const char *fragSrc)
{	
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSrc, nullptr);
	glCompileShader(vertShader);
	if (!didShaderCompile(vertShader, GL_VERTEX_SHADER))
		return (0);
	
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSrc, nullptr);
	glCompileShader(fragShader);
	if (!didShaderCompile(vertShader, GL_FRAGMENT_SHADER))
		return (0);
	
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	return (program);
}
