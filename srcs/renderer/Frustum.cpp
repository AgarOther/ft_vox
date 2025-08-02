#include "Frustum.hpp"
#include <glm/gtc/matrix_access.hpp>

Frustum::Frustum(const glm::mat4 & matrix)
{
	_planes[0] = glm::row(matrix, 3) + glm::row(matrix, 0);
	_planes[1] = glm::row(matrix, 3) - glm::row(matrix, 0);
	_planes[2] = glm::row(matrix, 3) + glm::row(matrix, 1);
	_planes[3] = glm::row(matrix, 3) -glm::row(matrix, 1);
	_planes[4] = glm::row(matrix, 3) + glm::row(matrix, 2);
	_planes[5] = glm::row(matrix, 3) - glm::row(matrix, 2);

	for (int i = 0; i < 6; ++i)
	{
		double length = glm::length(glm::vec3(_planes[i]));
		_planes[i] /= length;
	}
}

bool Frustum::isChunkVisible(const glm::vec3 & min, const glm::vec3 & max) const
{
	for (const auto & plane : _planes)
	{
		glm::vec3 positiveVertex = {
			(plane.x > 0) ? max.x : min.x,
			(plane.y > 0) ? max.y : min.y,
			(plane.z > 0) ? max.z : min.z,
		};

		if (glm::dot(glm::vec3(plane), positiveVertex) + plane.w < 0)
			return false;
	}
	return true;
}
