#ifndef CAKE_H
#define CAKE_H


#include <glm/glm.hpp>

#include <glload/gl_3_3.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>

#include "../SmallPhysics/SmallPhysics.h"
#include "../Sprite/Sprite.h"


class Cake
{
private:
	glm::vec2 position;

	float width;
	float height;

	float animationMaxY;
	float animationMinY;
	float animationOffset;

	float fatPoints;

	bool isEaten;
	bool isOutOfWindow;

	CollisionBody_AABB_2D cakeCollisionBody;

	
	Sprite cakeSprite;

public:
	Cake();
	Cake(glm::vec2 newPosition, 
	     float newAnimationMaxOffset,
		 float newAnimationOffset,
		 float newFatPoints,
		 float newWidth, float newHeight);

	void Init();


	void Update(float leftVelocity);

	void Render(GLuint shaderProgram);


	bool IsOutOfWindow()
	{
		return isOutOfWindow;
	}

	CollisionBody_AABB_2D GetCollisionBody()
	{
		return cakeCollisionBody;
	}

	glm::vec2 GetMinCorner()
	{
		return position;
	}
	glm::vec2 GetMaxCorner()
	{
		return position + glm::vec2(width, height);
	}

	float GetWidth()
	{
		return width;
	}
	float GetHeight()
	{
		return height;
	}

	float GetFat()
	{
		return fatPoints;
	}
	void SetIsEaten(bool newIsEaten)
	{
		isEaten = true;
	}

	bool IsEaten()
	{
		return isEaten;
	}
};


#endif