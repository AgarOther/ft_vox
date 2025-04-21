/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureType.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:35:41 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 02:43:23 by scraeyme         ###   ########.fr       */
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
			textures.push_back(new Texture("warped_nylium_top.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("warped_nylium_side.png", GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("netherrack.png", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case CRIMSON_NYLIUM:
		{
			textures.push_back(new Texture("crimson_nylium_top.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("crimson_nylium_side.png", GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("netherrack.png", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case GRASS_BLOCK:
		{
			textures.push_back(new Texture("grass_block_top.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("grass_block_side.png", GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("dirt.png", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case CRAFTING_TABLE:
		{
			textures.push_back(new Texture("crafting_table_top.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("crafting_table_side.png", GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("crafting_table_front.png", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("oak_planks.png", GL_TEXTURE_2D, 3, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case NETHERRACK:
		{
			textures.push_back(new Texture("netherrack.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case DIRT:
		{
			textures.push_back(new Texture("dirt.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case STONE:
		{
			textures.push_back(new Texture("stone.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case BEDROCK:
		{
			textures.push_back(new Texture("bedrock.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		default:
		{
			textures.push_back(new Texture("unknown.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
	}
	return (textures);
}

const GLuint *TextureType::getIndices(Material material)
{
	switch (material)
	{
		case UNKNOWN:
		case DIRT:
		case STONE:
		case BEDROCK:
		case NETHERRACK: return (textureId1s);
		case GRASS_BLOCK:
		case WARPED_NYLIUM:
		case CRIMSON_NYLIUM: return (textureId3s);
		case CRAFTING_TABLE: return (textureId4s);
		default : return (nullptr);
	}
}
