/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 11:53:46 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 14:12:17 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "colors.hpp"
#include <GL/glew.h>

void	debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	std::string color;
	std::string	error_type;

	// Ignore-list.
	// if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
	// 	return ;
	if (!userParam)
		(void) userParam;
	(void) length;
	color = (severity == (GL_DEBUG_SEVERITY_NOTIFICATION) ? WHITE :
			 severity == (GL_DEBUG_SEVERITY_LOW) ? CYAN :
			 severity == (GL_DEBUG_SEVERITY_MEDIUM) ? YELLOW : BOLD_RED);
	error_type = (color == WHITE ? "Notification" : color == CYAN ? "Warning" : color == YELLOW ? "Error" : "Critical");
	std::cout << color << "[GL " << error_type << "] Source: " << source << " | Type: " << type << " | ID: " << id << std::endl;
	std::cout << message << RESET << std::endl << std::endl;
}
