#include "types.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include <sstream>

Object getObjectFromFile(const std::string & filepath)
{
	ObjectType type;
	Object object;

	type = VERTICES;
	const std::string file = getFileAsString(filepath.c_str());
	if (file.empty())
		handleExit(FAILURE_OBJECT);
	std::istringstream lineBuffer(file);
	for (std::string line; std::getline(lineBuffer, line);)
	{
		if ((!line.empty() && line[0] == '#') || line.empty() || (!line.empty() && line[0] == '\n'))
			continue;
		else if (!line.empty() && line[0] == 'I')
		{
			type = INDICES;
			continue;
		}
		std::vector<std::string> tmp = ft_split(line, ' ');
		for (std::string str : tmp)
		{
			if (!str.empty() && str[0] == '#')
				continue;
			if (type == VERTICES)
				object.vertices.push_back(std::atof(str.c_str()));
			else
				object.indices.push_back(std::atoi(str.c_str()));
		}
	}
	return object;
}
