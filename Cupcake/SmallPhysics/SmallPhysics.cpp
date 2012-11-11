#include "stdafx.h"
#include "SmallPhysics.h"


CollisionBody_AABB_2D::CollisionBody_AABB_2D(glm::vec2 newCenter, 
						  float newHalfWidth, float newHalfHeight)
{
	center = newCenter;

	halfWidth = newHalfWidth;
	halfHeight = newHalfHeight;
}

bool CollisionBody_AABB_2D::IsCollided(const CollisionBody_AABB_2D &other)
{
	if(fabsf(center.x - other.center.x) >=
	   (halfWidth + other.halfWidth))
	   return false;
	if(fabsf(center.y - other.center.y) >=
	   (halfHeight + other.halfHeight))
	   return false;

	return true;
}