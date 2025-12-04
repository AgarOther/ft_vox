/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:27:04 by scraeyme          #+#    #+#             */
/*   Updated: 2025/09/01 17:32:17 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BlockTypeRegistry.hpp"
#include "Chunk.hpp"
#include "utils.hpp"

Material getMaterialFromString(std::string & name)
{
	for (char i = 0; i < (char)name.length(); i++)
		name[i] = std::tolower(name[i]);
	BlockTypeRegistry::BlockTypeMap blockTypeMap = BlockTypeRegistry::getBlockTypeMap();
	if (name == "air")
		return AIR;
	for (auto it = blockTypeMap.begin(); it != blockTypeMap.end(); ++it)
	{
		if (name == it->second.name)
			return (it->second.material);
	}
	return UNKNOWN;
}

void dispatchCommand(char * buffer, Player * player)
{
	std::string command;
	std::vector<std::string> args;

	command = buffer;
	args = ft_split(command, ' ');
	if (args.size() == 5 && command.rfind("setblock ", 0) == 0)
	{
		Chunk *chunk = player->getWorld()->getChunkAt(atoi(args[1].c_str()), atoi(args[3].c_str()));
		if (!chunk)
			return;
		chunk->changeBlockAt(Location(atoi(args[1].c_str()), atoi(args[2].c_str()), 
			atoi(args[3].c_str())), getMaterialFromString(args[4]));
	}
	else if (args.size() == 4 && command.rfind("tp ", 0) == 0)
		player->teleport(Location(atof(args[1].c_str()), atof(args[2].c_str()), atof(args[3].c_str())));
	else if (command.rfind("cw") == 0 && args.size() == 2)
	{
		World * world = WorldManager::getWorld(args[1]);
		if (world)
		{
			player->setWorld(world);
			world->generateProcedurally(true);
			player->checkFogChange();
		}
	}
}
