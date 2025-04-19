/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 21:49:29 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 12:26:22 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <string>
# include <GL/glew.h>
# define HEIGHT 800
# define WIDTH 800

class Utils
{
	public:
		static std::string getShaderAsString(const char *name);
		static int ft_error(int error_id, const std::string &error);
		static void setupGlfw();
		static GLuint compileShader(const char *vertSrc, const char *fragSrc);
		static void unbindAll();
};

void	debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

#endif
