/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EBO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:36:56 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 16:02:27 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EBO.hpp"

EBO::EBO(): _id(0) {}

EBO::EBO(const GLuint *indices, const GLsizeiptr size)
{
	glGenBuffers(1, &_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

GLuint EBO::getId() const
{
	return (_id);
}

void EBO::bind() const
{
	if (_id == 0)
		return;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void EBO::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::free() const
{
	if (_id == 0)
		return;
	glDeleteBuffers(1, &_id);
}
