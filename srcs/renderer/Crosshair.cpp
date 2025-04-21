/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Crosshair.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 01:18:30 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 01:48:57 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Crosshair.hpp"
#include "Utils.hpp"

const float crosshairVertices[] =
{
    -0.015f,  0.0f,
     0.015f,  0.0f,
     0.0f, -0.015f,
     0.0f,  0.015f,
};

Crosshair::Crosshair()
{
	this->_vao = VAO();
	this->_vao.bind();
	this->_shader = Shader("crosshair.vert", "crosshair.frag", false);
	this->_vbo = VBO(crosshairVertices, 8 * sizeof(float));
	this->_vao.linkAttribFloat(this->_vbo, 0, 2, GL_FLOAT, 2 * sizeof(float), 0);
	this->_vao.unbind();
}

void Crosshair::draw()
{
	glDisable(GL_DEPTH_TEST);
	this->_vao.bind();
	this->_shader.use();
	glDrawArrays(GL_LINES, 0, 4);
	this->_vao.unbind();
	glEnable(GL_DEPTH_TEST);
}
