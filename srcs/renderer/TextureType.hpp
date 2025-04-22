/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureType.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:35:10 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 13:15:14 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_TYPE
# define TEXTURE_TYPE

# include "Texture.hpp"

typedef enum e_material
{
	CRIMSON_NYLIUM,
	WARPED_NYLIUM,
	NETHERRACK,
	CRAFTING_TABLE,
	GRASS_BLOCK,
	DIRT,
	STONE,
	BEDROCK,
	EMERALD_BLOCK,
	CHERRY_LOG,
	DIAMOND_ORE,
	GOLD_ORE,
	IRON_ORE,
	COAL_ORE,
	COPPER_ORE,
	EMERALD_ORE,
	UNKNOWN
}	Material;

class TextureType
{
	public:
		static std::vector<Texture*> generateTextures(Material material);
		static const GLuint *getIndices(Material type);
};

#endif