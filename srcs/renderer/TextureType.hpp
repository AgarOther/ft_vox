/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureType.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:35:10 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 02:19:41 by scraeyme         ###   ########.fr       */
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
	UNKNOWN
}	Material;

class TextureType
{
	public:
		static std::vector<Texture*> generateTextures(Material material);
		static const GLuint *getIndices(Material type);
};

#endif