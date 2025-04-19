/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:38:03 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 22:47:16 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_HPP
# define SHADER_HPP

# include <GL/glew.h>
# include <glm/glm.hpp>
# include <glm/gtc/type_ptr.hpp>

class Shader
{
	public:
		Shader(const char *vertFile, const char *fragFile);

		GLuint getId() const;
		void use();
		void free();
		void setMat4(const char *uniform, glm::mat4 model);
		void setInt(const char *uniform, int n);
		static Shader &getCurrentlyBoundShader();
	private:
		GLuint _id;
};

#endif
