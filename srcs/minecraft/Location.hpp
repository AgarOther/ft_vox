/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:59:38 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 17:12:20 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

class Location
{
	public:
		Location();
		Location(float x, float y, float z);
		
		float getX() const;
		float getY() const;
		float getZ() const;

		void setX(float x);
		void setY(float y);
		void setZ(float z);
	private:
		float _x;	
		float _y;	
		float _z;	
};

#endif
