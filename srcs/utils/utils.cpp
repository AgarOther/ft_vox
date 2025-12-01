#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include "../includes/utils.hpp"
#include "colors.hpp"

std::vector<std::string> ft_split(const std::string & str, char delimiter)
{
	std::vector<std::string> tab;
	std::stringstream ss(str);
	std::string item;

	while (std::getline(ss, item, delimiter))
	{
		if (!item.empty())
			tab.push_back(item);
	}
	return tab;
}

void handleExit(int code, const std::string & message)
{
	std::cerr << message << std::endl;
	exit(code);
}

const std::string getShaderAsString(std::string path)
{
	if (path.substr(path.length() - 5, 5) == ".vert")
		path = "srcs/shaders/vertex/" + path;
	else
		path = "srcs/shaders/fragment/" + path;
	return getFileAsString(path.c_str());
}

const std::string getFileAsString(const char * name)
{
	std::ifstream	file;
	std::string		buffer;
	std::string		content;
	std::string		filename;
	
	filename = name;
	file.open(filename, std::ios::in);
	if (file.fail())
	{
		std::cout << BOLD_RED << "Couldn't find file " << filename << "." << RESET <<std::endl;
		exit(6);
	}
	while (std::getline(file, buffer))
		content += buffer + '\n';
	file.close();
	if (content.empty())
	{
		std::cout << BOLD_RED << "File " << filename << " is empty." <<  RESET <<std::endl;
		exit(6);
	}
	return (content);
}

long getTimeAsMilliseconds()
{
	std::chrono::time_point now = std::chrono::system_clock::now();
	auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	return std::chrono::duration_cast<std::chrono::milliseconds>(ms.time_since_epoch()).count();
}

Seed generateRandomSeed()
{
	static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<unsigned long> dist(0, std::numeric_limits<unsigned long>::max());
    return dist(gen);
}
