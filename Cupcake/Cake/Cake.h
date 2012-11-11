#ifndef CAKE_H
#define CAKE_H


#include <glm/glm.hpp>

#include <glload/gl_3_3.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>

#include "../SmallPhysics/SmallPhysics.h"


class Cake
{
private:
	glm::vec2 position;

	float width;
	float height;

	int fatPoints;

	bool isEaten;

	CollisionBody_AABB_2D cakeCollisionBody;


	GLuint vertexBufferObject;
	GLuint vao;

	float vertexData[24];

public:
	Cake();
	Cake(glm::vec2 newPosition, 
		 float newFatPoints,
		 float newWidth, float newHeight);

	void Init();


	void Update(float leftVelocity);

	void Render(GLuint shaderProgram);


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

	int GetFat()
	{
		return fatPoints;
	}
	void Eat()
	{
		isEaten = true;
	}
};


#endif