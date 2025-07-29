#pragma once

#include "types.hpp"
#include <unordered_map>

class ObjectRegistry
{
	public:
		static void				init();
		static const Object &	getObject(uint8_t id);
	private:
		typedef std::unordered_map<uint8_t, Object> ObjectMap;
		static ObjectMap 		_objects;
};
