/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:00:39 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 20:13:29 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : _x(0), _y(0), _z(0) {}

Location::Location(const double x, const double y, const double z) : _x(x), _y(y), _z(z) {}

bool Location::operator==(const Location &obj) const
{
	return (_x == obj._x && _y == obj._y && _z == obj._z);
}

double Location::getX() const { return _x; }
double Location::getY() const { return _y; }
double Location::getZ() const { return _z; }

void Location::setX(double x) { _x = x; }
void Location::setY(double y) { _y = y; }
void Location::setZ(double z) { _z = z; }
