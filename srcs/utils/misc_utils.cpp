/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 22:20:51 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 13:51:18 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <string>
#include "Utils.hpp"
#include "colors.hpp"
#include "srcs/minecraft/BlockData.hpp"

int Utils::getBlockIndex(const int x, const int y, const int z) {
	return (x + z * chunkSize + y * chunkSize * chunkSize);
}

std::string Utils::appendInt(std::string str, int n)
{
	str += std::to_string(n);
	return (str);
}

int Utils::ft_error(int error_id, const std::string &error)
{
	std::cerr << RED << error << RESET << std::endl;
	return (error_id);
}

static bool endsWith(const std::string &str, const std::string &suffix)
{
	int	i;
	int	j;

	i = str.length() - 1;
	j = suffix.length() - 1;
	if (i < j)
		return (false);
	while (i >= 0 && j >= 0)
	{
		if (str[i] != suffix[j])
			return (false);
		i--;
		j--;
	}
	return (true);
}

std::string Utils::getShaderAsString(const char *name)
{
	std::ifstream	file;
	std::string		buffer;
	std::string		shader;
	std::string		filename;
	
	filename = name;
	if (endsWith(filename, ".vert"))
		filename = "shaders/vertex/" + filename;
	else
		filename = "shaders/fragment/" + filename;
	file.open(filename, std::ios::in);
	if (file.fail())
	{
		std::cout << BOLD_RED << "Couldn't find shader file " << filename << "." << std::endl;
		exit(6);
	}
	while (std::getline(file, buffer))
		shader += buffer + '\n';
	file.close();
	if (shader.empty())
	{
		std::cout << BOLD_RED << "File " << filename << " is empty." << std::endl;
		exit(6);
	}
	return (shader);
}
