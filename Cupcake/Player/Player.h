#ifndef PLAYER_H
#define PLAYER_H


#include <glm/glm.hpp>

#include <glload/gl_3_3.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>


#include "../Framework/Framework.h"
#include "../Platform/Platform.h"


#include <vector>


static float gravity = 0.0000005f;


class Player
{
private:
	glm::vec2 currentPosition;
	glm::vec2 oldPosition;

	glm::vec2 velocity;

	glm::vec2 offset;

	
	float width;
	float height;


	int fatCount;


	GLuint vertexBufferObject;
	GLuint vao;

	float vertexData[24];


	bool isJumping;


	void IsCollided(Platform &platform);

	void UpdatePhysics(Platform platforms[], int numCorners);
	void UpdatePositions();
	

public:
	Player();
	Player(glm::vec2 newPosition, glm::vec2 newVelocity,
		   float newWidth, float newHeight,
		   int newFatCount);

	void Init();

	void Update(Platform platforms[], int numCorners);

	void Render(GLuint shaderProgram);

	void MoveLeft();
	void MoveRight();

	void Jump(float units);


	glm::vec2 GetMinCorner()
	{
		return currentPosition;
	}
	glm::vec2 GetMaxCorner()
	{
		return currentPosition + glm::vec2(width, height);
	}

	float GetWidth()
	{
		return width;
	}
	float GetHeight()
	{
		return height;
	}

	void AddFat(int fatToAdd)
	{
		fatCount += fatToAdd;
	}
};


#endif