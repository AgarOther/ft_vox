/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VAO.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:35:51 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/18 23:36:31 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VAO_HPP
# define VAO_HPP
# include <glad/glad.h>
# include "VBO.hpp"

class VAO
{
	public:
		VAO();

		GLuint getId() const;
		void linkAttrib(VBO &vbo, GLuint layout, GLuint nbComponents, GLenum type, GLsizeiptr stride, void *offset);
		void bind();
		void unbind();
		void free();
	private:
		GLuint _id;
};

#endif
