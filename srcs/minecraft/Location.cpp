/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:00:39 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 17:12:09 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
{
	this->_x = 0.0f;
	this->_y = 0.0f;
	this->_z = 0.0f;
}

Location::Location(float x, float y, float z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
}

float Location::getX() const { return this->_x; }
float Location::getY() const { return this->_y; }
float Location::getZ() const { return this->_z; }

void Location::setX(float x) { this->_x = x; }
void Location::setY(float y) { this->_y = y; }
void Location::setZ(float z) { this->_z = z; }
