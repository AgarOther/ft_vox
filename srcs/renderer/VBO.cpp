/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VBO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:37:20 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 13:56:33 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VBO.hpp"

VBO::VBO(): _id(0) {}

GLuint VBO::getId() const
{
	return (this->_id);
}

void VBO::bind() const
{
	if (_id == 0)
		return;
	glBindBuffer(GL_ARRAY_BUFFER, this->_id);
}

void VBO::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::free() const
{
	if (_id == 0)
		return;
	glDeleteBuffers(1, &this->_id);
}
