#include "utils.hpp"
#include <iostream>
#include "colors.hpp"

void	debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	std::string color;
	std::string	error_type;

	// Ignore-list.
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;
	if (!userParam)
		(void) userParam;
	(void) length;
	color = (severity == (GL_DEBUG_SEVERITY_NOTIFICATION) ? MAGENTA :
			 severity == (GL_DEBUG_SEVERITY_LOW | GL_DEBUG_SEVERITY_LOW_AMD | GL_DEBUG_SEVERITY_LOW_ARB) ? CYAN :
			 severity == (GL_DEBUG_SEVERITY_MEDIUM | GL_DEBUG_SEVERITY_MEDIUM_AMD | GL_DEBUG_SEVERITY_MEDIUM_ARB) ? YELLOW : BOLD_RED);
	error_type = (color == MAGENTA ? "Notification" : color == CYAN ? "Warning" : color == YELLOW ? "Error" : "Critical");
	std::cout << color << "[GL " << error_type << "] Source: " << source << " | Type: " << type << " | ID: " << id << std::endl;
	std::cout << message << RESET << std::endl << std::endl;
}
