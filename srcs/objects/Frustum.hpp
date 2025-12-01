#pragma once
#include "glm/glm.hpp"

class Frustum
{
	public:
		Frustum(const glm::mat4 & matrix);
		
		bool isChunkVisible(const glm::vec3 & min, const glm::vec3 & max) const;
	private:
		glm::vec4 _planes[6];
};
