/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureType.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:35:41 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 16:03:18 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TextureType.hpp"
#include "../minecraft/BlockData.hpp"

std::vector<Texture*> TextureType::generateTextures(Material material)
{
	std::vector<Texture*> textures;

	switch (material)
	{
		case WARPED_NYLIUM:
		{
			textures.push_back(new Texture("textures/warped_nylium_top.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("textures/warped_nylium_side.png", GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("textures/netherrack.png", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		default:
		{
			textures.push_back(new Texture("textures/netherrack.png", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
	}
	return (textures);
}

const GLuint *TextureType::getIndices(Material material)
{
	switch (material)
	{
		case NETHERRACK: return (textureId1s);
		case WARPED_NYLIUM: return (textureId3s);
		default : return (nullptr);
	}
}
