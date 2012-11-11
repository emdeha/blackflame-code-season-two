#ifndef PLATFORM_H
#define PLATFORM_H


#include <glm/glm.hpp>

#include <glload/gl_3_3.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>

#include "../Framework/Framework.h"


#include <vector>


class Platform
{
private:
	glm::vec2 position;

	float leftMovementVelocity;


	float width;
	float height;

	bool isOutOfWindow;



	GLuint vertexBufferObject;
	GLuint vao;

	float vertexData[24];

public:
	Platform();

	Platform(glm::vec2 newPosition,
			 float newWidth, float newHeight,
			 float newLeftMovementVelocity);

	void Init();

	void Update();

	void Render(GLuint shaderProgram);


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