/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:38:03 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 01:36:33 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_HPP
# define SHADER_HPP

# include <GL/glew.h>
# include <glm/glm.hpp>
# include <glm/gtc/type_ptr.hpp>
# include "TextureType.hpp"

class Shader
{
	public:
		Shader();
		Shader(const char *vertFile, const char *fragFile, bool changeBound);

		GLuint getId() const;
		void use();
		void free();
		void setMat4(const char *uniform, glm::mat4 model);
		void setVec4(const char *uniform, glm::vec4 vec);
		void setInt(const char *uniform, int n);
		void setFloat(const char *uniform, float f);
		void setTint(Material material);

		static Shader &getCurrentlyBoundShader();
	private:
		GLuint _id;
};

#endif
