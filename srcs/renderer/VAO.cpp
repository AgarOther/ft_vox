/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VAO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:37:08 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 13:14:50 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VAO.hpp"

VAO::VAO()
{
	glGenVertexArrays(1, &this->_id);
}

GLuint VAO::getId() const
{
	return (this->_id);
}

void VAO::linkAttribFloat(VBO &vbo, const GLuint layout, const GLint nbComponents, const GLenum type, const GLsizei stride, const void *offset) const
{
	vbo.bind();
	glVertexAttribPointer(layout, nbComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}

void VAO::linkAttribInt(VBO &vbo, const GLuint layout, const GLint nbComponents, const GLenum type, const GLsizei stride, const void *offset) const
{
	vbo.bind();
	glVertexAttribIPointer(layout, nbComponents, type, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}

void VAO::bind() const
{
	glBindVertexArray(this->_id);
}

void VAO::unbind() const
{
	glBindVertexArray(0);
}

void VAO::free() const
{
	glDeleteVertexArrays(1, &this->_id);
}
