#pragma once

#include <glm/vec3.hpp>
#include "Location.hpp"

class BoundingBox
{
	public:
		BoundingBox() : _a(glm::vec3()), _b(glm::vec3()) {}
		BoundingBox(const Location & a, const Location & b);

		bool				operator==(const BoundingBox & obj);
		bool				operator!=(const BoundingBox & obj);

		bool				isColliding(const BoundingBox & bb);

		const glm::vec3	&	getA() const { return _a; };
		const glm::vec3	&	getB() const { return _b; };
	private:
		glm::vec3			_a;
		glm::vec3			_b;
};
