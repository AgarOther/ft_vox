#include "BoundingBox.hpp"

bool BoundingBox::operator==(const BoundingBox & obj)
{
	return (_a == obj.getA() && _b == obj.getB());
}

bool BoundingBox::operator!=(const BoundingBox & obj)
{
	return (_a != obj.getA() || _b != obj.getB());
}

bool BoundingBox::isColliding(const BoundingBox & bb)
{
	return (_a.x >= bb.getA().x && bb.getA().x <= _b.x)
			&& (_a.x >= bb.getB().x && bb.getB().x <= _b.x)
			&& (_a.y >= bb.getA().y && bb.getA().y <= _b.y)
			&& (_a.y >= bb.getB().y && bb.getB().y <= _b.y)
			&& (_a.z >= bb.getA().z && bb.getA().z <= _b.z)
			&& (_a.z >= bb.getB().z && bb.getB().z <= _b.z);
}
