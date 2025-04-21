/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:35:01 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 03:26:16 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_HPP
# define BLOCK_HPP

# include "../renderer/Texture.hpp"
# include "../renderer/TextureType.hpp"
# include "../renderer/VAO.hpp"
# include "../renderer/EBO.hpp"
# include "../renderer/Shader.hpp"
# include "Location.hpp"
# include <vector>

class Block
{
	public:
		Block();
		Block(Material type);
		Block &operator=(const Block &obj);

		void free();
		void draw(Shader &shader);
		void placeBlockAt(const Location &location);
		void placeBlockAt(const Location &location, Shader &shader);

		const Material &getType() const;
		void setType(Material &material);
	private:
		std::vector<Texture *> _textures;
		VAO _vao;
		VBO _vbo;
		VBO _texVbo;
		VBO _blockFacesVbo;
		VBO _texDataVbo;
		EBO _ebo;
		Material _material;
};

#endif