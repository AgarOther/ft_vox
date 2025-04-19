/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:38:03 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 12:23:19 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_HPP
# define SHADER_HPP

#include <GL/glew.h>

class Shader
{
	public:
		Shader(const char *vertFile, const char *fragFile);

		GLuint getId() const;
		void use();
		void free();
	private:
		GLuint _id;
};

#endif
