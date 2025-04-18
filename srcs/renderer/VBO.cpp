/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VBO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:37:20 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/18 23:36:25 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VBO.hpp"

VBO::VBO(GLfloat *vertices, GLsizeiptr size)
{
	glGenBuffers(1, &this->_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->_id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

GLuint VBO::getId() const
{
	return (this->_id);
}

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->_id);
}

void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::free()
{
	glDeleteBuffers(1, &this->_id);
}
