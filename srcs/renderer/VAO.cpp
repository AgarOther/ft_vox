/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VAO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:37:08 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/18 23:36:22 by scraeyme         ###   ########.fr       */
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

void VAO::linkAttrib(VBO &vbo, GLuint layout, GLuint nbComponents, GLenum type, GLsizeiptr stride, void *offset)
{
	vbo.bind();
	glVertexAttribPointer(layout, nbComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}

void VAO::bind()
{
	glBindVertexArray(this->_id);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}

void VAO::free()
{
	glDeleteVertexArrays(1, &this->_id);
}
