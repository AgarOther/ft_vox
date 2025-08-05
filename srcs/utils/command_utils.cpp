/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:27:04 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 23:21:41 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"
#include "types.hpp"
#include "utils.hpp"

static Material getMaterialFromString(std::string name)
{
	for (char i = 0; i < (char)name.length(); i++)
		name[i] = std::tolower(name[i]);
	if (name == "bedrock")
		return (BEDROCK);
	else if (name == "stone")
		return (STONE);
	else if (name == "dirt")
		return (DIRT);
	else if (name == "acacia_log")
		return (ACACIA_LOG);
	else if (name == "sand")
		return (SAND);
	else if (name == "end_stone")
		return (END_STONE);
	else if (name == "cartography_table")
		return (CARTOGRAPHY_TABLE);
	else
		return (UNKNOWN);
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
}
