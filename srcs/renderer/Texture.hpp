/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:20:22 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/18 23:38:09 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
# define TEXTURE_HPP
# include <glad/glad.h>
# include "Shader.hpp"

class Texture
{
	public:
		Texture(const char *image, GLenum textureType, GLenum slot, GLenum format, GLenum pixeltype);

		GLuint getId() const;
		GLenum getType() const;
		void setSlot(Shader &shader, const char *uniform, GLuint unit);
		void bind();
		void unbind();
		void free();
	private:
		GLuint _id;
		GLenum _type;
};

#endif