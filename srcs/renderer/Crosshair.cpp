/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Crosshair.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 01:18:30 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/26 21:52:04 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Crosshair.hpp"
#include "../utils/Utils.hpp"

constexpr float crosshairVertices[] =
{
	-0.015f,  0.0f,
	 0.015f,  0.0f,
	 0.0f, -0.015f,
	 0.0f,  0.015f,
};

Crosshair::Crosshair()
{
	_vao = VAO();
	_vao.bind();
	_shader = Shader("crosshair.vert", "crosshair.frag", false);
	_vbo = VBO(crosshairVertices, 8 * sizeof(float));
	_vao.linkAttribFloat(_vbo, 0, 2, GL_FLOAT, 2 * sizeof(float), nullptr);
	_vao.unbind();
}

void Crosshair::draw()
{
	glDisable(GL_DEPTH_TEST);
	_vao.bind();
	_shader.use();
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_shader.setFloat("aspectRatio", static_cast<float>(WIDTH) / static_cast<float>(HEIGHT));
	glDrawArrays(GL_LINES, 0, 4);
	_vao.unbind();
	glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
