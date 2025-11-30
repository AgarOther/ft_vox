#include "Location.hpp"
#include <cmath>

Location & Location::add(const Location & loc)
{
	_x += loc._x;
	_y += loc._y;
	_z += loc._z;
	return *this;
}

Location & Location::add(double x, double y, double z)
{
	_x += x;
	_y += y;
	_z += z;
	return *this;
}

Location & Location::add(int x, int y, int z)
{
	_x += static_cast<double>(x);
	_y += static_cast<double>(y);
	_z += static_cast<double>(z);
	return *this;
}

Location & Location::sub(const Location & loc)
{
	_x -= loc._x;
	_y -= loc._y;
	_z -= loc._z;
	return *this;
}

Location & Location::sub(double x, double y, double z)
{
	_x -= x;
	_y -= y;
	_z -= z;
	return *this;
}

Location & Location::sub(int x, int y, int z)
{
	_x -= static_cast<double>(x);
	_y -= static_cast<double>(y);
	_z -= static_cast<double>(z);
	return *this;
}

double Location::distance(const Location & loc) const
{
	return (sqrt(pow(loc.getX() - _x, 2) + pow(loc.getY() - _y, 2) + pow(loc.getZ() - _z, 2)));
}

Location & Location::blockalize()
{
	_x = static_cast<int>(std::floor(_x));
	_y = static_cast<int>(std::floor(_y));
	_z = static_cast<int>(std::floor(_z));
	return (*this);
}

Location Location::clone() const
{
	return Location(*this);
}

Location & Location::operator=(const Location & loc)
{
	if (this != &loc)
	{
		_x = loc._x;
		_y = loc._y;
		_z = loc._z;
	}
	return *this;
}

Location Location::operator+(const Location & loc) const
{
	return Location(_x + loc._x, _y + loc._y, _z + loc._z);
}

Location & Location::operator+=(const Location & loc)
{
	_x += loc._x;
	_y += loc._y;
	_z += loc._z;
	return *this;
}

Location Location::operator-(const Location & loc) const
{
	return Location(_x - loc._x, _y - loc._y, _z - loc._z);
}

Location & Location::operator-=(const Location & loc)
{
	_x -= loc._x;
	_y -= loc._y;
	_z -= loc._z;
	return *this;
}

bool Location::operator==(const Location & loc) const
{
	if (this == &loc)
		return true;
	return _x == loc._x && _y == loc._y && _z == loc._z;
}

bool Location::operator!=(const Location & loc) const
{
	if (this == &loc)
		return false;
	return _x != loc._x || _y != loc._y || _z != loc._z;
}

glm::vec3 Location::getVec3() const
{
	return glm::vec3(_x, _y, _z);
}

std::ostream& operator<<(std::ostream& os, const Location& loc)
{
	return os << "(" << loc.getX() << ", " << loc.getY() << ", " << loc.getZ() << ")";
}
