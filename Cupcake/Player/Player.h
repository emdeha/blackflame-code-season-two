#ifndef PLAYER_H
#define PLAYER_H


#include <ctime>


#include "../Platform/Platform.h"
#include "../SmallPhysics/SmallPhysics.h"
#include "../Sprite/Sprite.h"


#include <vector>


static float gravity = 0.0000009f;


class Player
{
private:
	glm::vec2 currentPosition;
	glm::vec2 oldPosition;

	glm::vec2 velocity;

	glm::vec2 offset;


	double starvingTime_secs;
	clock_t lastTime;

	
	float width;
	float height;

	float jumpUnits;


	float fatCount;

	CollisionBody_AABB_2D playerCollisionBody;


	Sprite playerSprite;


	bool isJumping;
	bool isDead;


	void IsCollided(Platform &platform);

	void UpdatePhysics(Platform platforms[], int numCorners);
	void UpdatePositions();
	

public:
	Player();
	Player(glm::vec2 newPosition, glm::vec2 newVelocity,
		   double newStarvingTime_secs,
		   float newJumpUnits,
		   float newWidth, float newHeight);

	void Init();

	void Update(Platform platforms[], int numCorners);

	void Render(GLuint shaderProgram);

	void MoveLeft();
	void MoveRight();

	void Jump();


	CollisionBody_AABB_2D GetCollisionBody()
	{
		return playerCollisionBody;
	}

	glm::vec2 GetMinCorner()
	{
		return currentPosition;
	}
	glm::vec2 GetMaxCorner()
	{
		return currentPosition + glm::vec2(width, height);
	}

	glm::vec2 GetVelocity()
	{
		return velocity;
	}
	glm::vec2 GetPosition()
	{
		return currentPosition;
	}

	float GetWidth()
	{
		return width;
	}
	float GetHeight()
	{
		return height;
	}

	void AddFat(float fatToAdd)
	{
		fatCount += fatToAdd;
	}

	float GetFat()
	{
		return fatCount;
	}

	bool IsDead()
	{
		return isDead;
	}
};


#endif