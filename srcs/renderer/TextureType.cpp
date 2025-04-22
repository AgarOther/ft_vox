/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureType.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:35:41 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 13:15:34 by scraeyme         ###   ########.fr       */
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
		case EMERALD_BLOCK:
		{
			textures.push_back(new Texture("emerald_block.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case DIAMOND_ORE:
		{
			textures.push_back(new Texture("diamond_ore.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case GOLD_ORE:
		{
			textures.push_back(new Texture("gold_ore.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case IRON_ORE:
		{
			textures.push_back(new Texture("iron_ore.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case COAL_ORE:
		{
			textures.push_back(new Texture("coal_ore.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case COPPER_ORE:
		{
			textures.push_back(new Texture("copper_ore.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case EMERALD_ORE:
		{
			textures.push_back(new Texture("emerald_ore.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			break;
		}
		case CHERRY_LOG:
		{
			textures.push_back(new Texture("cherry_log_top.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE));
			textures.push_back(new Texture("cherry_log.png", GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE));
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
		case EMERALD_BLOCK:
		case DIAMOND_ORE:
		case GOLD_ORE:
		case IRON_ORE:
		case COAL_ORE:
		case NETHERRACK: return (textureId1s);
		case CHERRY_LOG: return (textureId2s);
		case GRASS_BLOCK:
		case WARPED_NYLIUM:
		case CRIMSON_NYLIUM: return (textureId3s);
		case CRAFTING_TABLE: return (textureId4s);
		default : return (nullptr);
	}
}
