/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EBO.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:36:28 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 16:02:24 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EBO_HPP
# define EBO_HPP
# include <GL/glew.h>

class EBO
{
	public:
		EBO();
		EBO(const GLuint *indices, GLsizeiptr size);

		GLuint getId() const;
		void bind();
		void unbind();
		void free();
	private:
		GLuint _id;
};

#endif
