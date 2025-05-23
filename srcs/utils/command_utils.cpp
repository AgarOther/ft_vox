/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:27:04 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/26 21:41:05 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "../renderer/TextureType.hpp"
#include <sstream>

static std::vector<std::string> ft_split(const std::string &str, const char c)
{
	std::vector<std::string> strs;
	std::stringstream ss(str);
	std::string buf;

	while (std::getline(ss, buf, c))
		strs.push_back(buf);
	return (strs);
}

// static Material getMaterialFromString(std::string name)
// {
// 	for (char i = 0; i < (char)name.length(); i++)
// 		name[i] = std::tolower(name[i]);
// 	if (name == "grass_block")
// 		return (Material::GRASS_BLOCK);
// 	else if (name == "cherry_log")
// 		return (Material::CHERRY_LOG);
// 	else if (name == "emerald_block")
// 		return (Material::EMERALD_BLOCK);
// 	else if (name == "bedrock")
// 		return (Material::BEDROCK);
// 	else if (name == "stone")
// 		return (Material::STONE);
// 	else if (name == "dirt")
// 		return (Material::DIRT);
// 	else if (name == "crafting_table")
// 		return (Material::CRAFTING_TABLE);
// 	else if (name == "netherrack")
// 		return (Material::NETHERRACK);
// 	else if (name == "warped_nylium")
// 		return (Material::WARPED_NYLIUM);
// 	else if (name == "crimson_nylium")
// 		return (Material::CRIMSON_NYLIUM);
// 	else
// 		return (Material::UNKNOWN);
// }

void Utils::dispatchCommand(Camera &camera, const char *buffer)
{
	// int chunkX;
	// int chunkZ;
	char *end;

	const std::string command = buffer;
	const std::vector<std::string> args = ft_split(command, ' ');
	// if (args.size() == 5 && command.rfind("setblock ", 0) == 0)
	// {
	// 	chunkX = std::floor(atoi(args[1].c_str()) / 16.0f);
	// 	chunkZ = std::floor(atoi(args[3].c_str()) / 16.0f);
	// 	if (chunkX > chunkSize)
	// 		chunkX /= chunkSize;
	// 	else if (chunkX < chunkSize)
	// 		chunkX /= -chunkSize;
	// 	Chunk *chunk = Chunk::getChunk(chunkX, chunkZ);
	// 	Block block(getMaterialFromString(args[4]), Location(atoi(args[1].c_str()), atoi(args[2].c_str()), atoi(args[3].c_str())), chunkX, chunkZ);
	// 	chunk->addBlock(block);
	// }
	if ((args.size() == 4 || args.size() == 6) && command.rfind("tp ", 0) == 0)
	{
		if (args.size() == 4)
			camera.teleport(Location(strtod(args[1].c_str(), &end), strtod(args[2].c_str(), &end), strtod(args[3].c_str(), &end)),
				camera.getYaw(), camera.getPitch());
		else
			camera.teleport(Location(strtod(args[1].c_str(), &end), strtod(args[2].c_str(), &end), strtod(args[3].c_str(), &end)),
				strtof(args[4].c_str(), &end), strtof(args[5].c_str(), &end));
		(void) end;
	}
}
