/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EBO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:36:56 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/18 23:36:07 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EBO.hpp"

EBO::EBO(GLuint *indices, GLsizeiptr size)
{
	glGenBuffers(1, &this->_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

GLuint EBO::getId() const
{
	return (this->_id);
}

void EBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_id);
}

void EBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::free()
{
	glDeleteBuffers(1, &this->_id);
}
