/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:35:01 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 17:14:43 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_HPP
# define BLOCK_HPP

# include "../renderer/Texture.hpp"
# include "../renderer/TextureType.hpp"
# include "../renderer/VAO.hpp"
# include "../renderer/EBO.hpp"
# include "Location.hpp"
# include <vector>

class Block
{
	public:
		Block(Material type);

		void free();
		void draw(Shader &shader);
		void placeBlockAt(const Location &location, Shader &shader);
	private:
		std::vector<Texture *> _textures;
		VAO _vao;
		VBO _vbo;
		VBO _texVbo;
		EBO _ebo;
};

#endif