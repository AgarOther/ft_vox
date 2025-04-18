/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VBO.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:36:44 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/18 23:36:29 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VBO_HPP
# define VBO_HPP
# include <glad/glad.h>

class VBO
{
	public:
		VBO(GLfloat *vertices, GLsizeiptr size);

		GLuint getId() const;
		void bind();
		void unbind();
		void free();
	private:
		GLuint _id;
};

#endif
