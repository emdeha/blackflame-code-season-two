#ifndef SMALL_PHYSICS_H
#define SMALL_PHYSICS_H


#include <glm/glm.hpp>

class CollisionBody_AABB_2D
{
private:
	glm::vec2 center;
	
	float halfWidth;
	float halfHeight;

public:
	CollisionBody_AABB_2D() {}
	CollisionBody_AABB_2D(glm::vec2 newCenter, 
						  float newHalfWidth, float newHalfHeight);

	bool IsCollided(const CollisionBody_AABB_2D &other);
};

#endif