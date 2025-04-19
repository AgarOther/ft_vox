/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VBO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:37:20 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 13:19:08 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VBO.hpp"

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
