#pragma once

#include <include/glm.h>

namespace Transform2D
{
	// Translate matrix
	inline glm::mat3 Translate(float tx, float ty)
	{
		// TODO implement translate matrix
		return glm::transpose (glm::mat3(1, 0, tx, 
										 0, 1, ty,
										 0, 0, 1));
	}

	// Scale matrix
	inline glm::mat3 Scale(float sX, float sY)
	{
		// TODO implement scale matrix
		return glm::mat3(sX, 0, 0,
						 0, sY, 0,
						 0, 0, 1);
	}

	// Rotate matrix
	inline glm::mat3 Rotate(float radians)
	{
		// TODO implement rotate matrix
		float c = cos(radians);
		float s = sin(radians);
		return glm::transpose (glm::mat3(c, -s, 0,
										 s, c, 0,
										 0, 0, 1));
	}
}
