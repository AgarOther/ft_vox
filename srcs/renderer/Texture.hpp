/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:20:22 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 02:47:11 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
# define TEXTURE_HPP
# include <GL/glew.h>

class Texture
{
	public:
		Texture(const char *image, GLenum textureType, GLenum slot, GLenum format, GLenum pixeltype);

		GLuint getId() const;
		GLenum getType() const;
		void bind() const;
		void unbind() const;
		void free() const;

		static void resetSlots();
	private:
		GLuint _id;
		GLenum _type;
};
#endif