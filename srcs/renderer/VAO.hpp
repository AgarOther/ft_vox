/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VAO.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:35:51 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 13:13:28 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VAO_HPP
# define VAO_HPP
# include <GL/glew.h>
# include "VBO.hpp"

class VAO
{
	public:
		VAO();

		GLuint getId() const;
		void linkAttribFloat(VBO &vbo, GLuint layout, GLint nbComponents, GLenum type, GLsizei stride, const void *offset) const;
		void linkAttribInt(VBO &vbo, GLuint layout, GLint nbComponents, GLenum type, GLsizei stride, const void *offset) const;
		void bind() const;
		void unbind() const;
		void free() const;
	private:
		GLuint _id;
};

#endif
