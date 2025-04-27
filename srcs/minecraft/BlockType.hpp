/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockType.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:35:01 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 23:50:59 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCKTYPE_HPP
# define BLOCKTYPE_HPP

# include "../renderer/Texture.hpp"
# include "../renderer/TextureType.hpp"
# include "../renderer/VAO.hpp"
# include "../renderer/EBO.hpp"
# include "../renderer/Shader.hpp"
# include <unordered_map>
# include <vector>

typedef enum e_blockface
{
	NORTH,
	SOUTH,
	WEST,
	EAST,
	UP,
	DOWN
}			BlockFace;

class Block;

class BlockType
{
	public:
		BlockType();
		BlockType(Material material);
		BlockType &operator=(const BlockType &obj);
		
		static void init();
		static void shutdown();
		void free() const;
		static void draw(const Material &material, const Shader &shader, uint8_t faceMask);
		
		const Material &getType() const;
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

extern std::unordered_map<Material, BlockType> blockMap;

#endif