#ifndef PLATFORM_H
#define PLATFORM_H



#include "../SmallPhysics/SmallPhysics.h"
#include "../Sprite/Sprite.h"


#include <vector>


class Platform
{
private:
	glm::vec2 position;

	float leftMovementVelocity;


	float width;
	float height;

	bool isOutOfWindow;


	CollisionBody_AABB_2D platformCollisionBody;

	Sprite platformSprite;

public:
	Platform();

	Platform(glm::vec2 newPosition,
			 float newWidth, float newHeight,
			 float newLeftMovementVelocity);

	void Init();

	void Update();

	void Render(GLuint shaderProgram);


	CollisionBody_AABB_2D GetCollisionBody()
	{
		return platformCollisionBody;
	}

	bool IsOutOfWindow()
	{
		return isOutOfWindow;
	}

	float GetLeftVelocity()
	{
		return leftMovementVelocity;
	}

	float GetWidth()
	{
		return width;
	}
	float GetHeight()
	{
		return height;
	}

	glm::vec2 GetMinCorner()
	{
		return position;
	}
	glm::vec2 GetMaxCorner()
	{
		return position + glm::vec2(width, height);
	}
};


#endif