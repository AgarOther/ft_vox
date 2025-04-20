/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:20:22 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/20 04:22:14 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
# define TEXTURE_HPP
# include <GL/glew.h>
# include <vector>
# include "Shader.hpp"

class Texture
{
	public:
		Texture();
		Texture(const char *image, GLenum textureType, GLenum slot, GLenum format, GLenum pixeltype);

		GLuint getId() const;
		GLenum getType() const;
		void bind();
		void unbind();
		void free();
		void addTint(float r, float g, float b);

		static void resetSlots();
	private:
		GLuint _id;
		GLenum _type;
};
#endif