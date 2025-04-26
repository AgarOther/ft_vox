/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VBO.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:36:44 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 13:56:22 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VBO_HPP
# define VBO_HPP
# include <GL/glew.h>

class VBO
{
	public:
		VBO();
		template<typename T>
		VBO(T *vertices, GLsizeiptr size)
		{
			glGenBuffers(1, &this->_id);
			glBindBuffer(GL_ARRAY_BUFFER, this->_id);
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}

		GLuint getId() const;
		void bind() const;
		void unbind() const;
		void free() const;
	private:
		GLuint _id;
};

#endif
