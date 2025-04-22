/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:59:38 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 20:13:40 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

class Location
{
	public:
		Location();
		Location(double x, double y, double z);

		bool operator==(const Location &obj);
		
		double getX() const;
		double getY() const;
		double getZ() const;

		void setX(double x);
		void setY(double y);
		void setZ(double z);
	private:
		double _x;	
		double _y;	
		double _z;	
};

#endif
