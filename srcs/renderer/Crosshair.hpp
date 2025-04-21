/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Crosshair.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 01:18:34 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 01:22:32 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CROSSHAIR_HPP
# define CROSSHAIR_HPP

# include "VAO.hpp"
# include "Shader.hpp"

class Crosshair
{
	public:
		Crosshair();

		void draw();
	private:
		VAO _vao;
		VBO _vbo;
		Shader _shader;
};

#endif
