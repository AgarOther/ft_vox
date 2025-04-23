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

Location::Location()
{
	this->_x = 0.0f;
	this->_y = 0.0f;
	this->_z = 0.0f;
}

Location::Location(double x, double y, double z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
}

bool Location::operator==(const Location &obj) const
{
	return (this->_x == obj._x && this->_y == obj._y && this->_z == obj._z);
}

double Location::getX() const { return this->_x; }
double Location::getY() const { return this->_y; }
double Location::getZ() const { return this->_z; }

void Location::setX(double x) { this->_x = x; }
void Location::setY(double y) { this->_y = y; }
void Location::setZ(double z) { this->_z = z; }
