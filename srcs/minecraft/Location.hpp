#pragma once
#include <ostream>
#include <glm/glm.hpp>

class Location
{
	public:
		Location(double x, double y, double z): _x(x), _y(y), _z(z) {}
		Location(int x, int y, int z): Location(static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)) {}
		Location(const Location & cpy): _x(cpy._x), _y(cpy._y), _z(cpy._z) {}

		Location &	operator=(const Location & loc);
		Location 	operator+(const Location & loc) const;
		Location &  operator+=(const Location & loc);
		Location 	operator-(const Location & loc) const;
		Location &  operator-=(const Location & loc);
		bool		operator==(const Location & loc) const;
		bool		operator!=(const Location & loc) const;

		double		getX() const { return _x; }
		double		getY() const { return _y; }
		double		getZ() const { return _z; }

		void		setX(double x) { _x = x; }
		void		setY(double y) { _y = y; }
		void		setZ(double z) { _z = z; }

		Location &	add(const Location & loc);
		Location &	add(double x, double y, double z);
		Location &	add(int x, int y, int z);

		glm::vec3	getVec3() const;
	private:
		double		_x;
		double		_y;
		double		_z;
};

std::ostream&		operator<<(std::ostream& os, const Location& loc);
