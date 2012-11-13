#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	currentPosition = glm::vec2();
	velocity = glm::vec2();
	offset = glm::vec2();

	width = 0.0f;
	height = 0.0f;

	isJumping = false;

	fatCount = 0;

	playerCollisionBody = CollisionBody_AABB_2D(glm::vec2(), 0.0f, 0.0f);
}
Player::Player(glm::vec2 newPosition, glm::vec2 newVelocity,
			   float newWidth, float newHeight,
		       int newFatCount)
{
	currentPosition = newPosition;
	velocity = newVelocity;

	offset = glm::vec2();

	width = newWidth;
	height = newHeight;

	isJumping = false;

	fatCount = newFatCount;

	glm::vec2 collisionBodyCenter = currentPosition + glm::vec2(width / 2.0f, height / 2.0f);
	playerCollisionBody = CollisionBody_AABB_2D(collisionBodyCenter, width / 2.0f, height / 2.0f);


	playerSprite = Sprite(currentPosition, glm::vec4(0.3f, 1.0f, 0.5f, 1.0f), width, height);
}


void Player::IsCollided(Platform &platform)
{
	if(!platform.GetCollisionBody().IsCollided(playerCollisionBody))
		return;
		
	float deltaX = 0.0f;
	float deltaY = 0.0f;

	glm::vec2 desiredNormal;

	glm::vec2 playerMaxCorner = currentPosition + glm::vec2(width, height);
	glm::vec2 playerMinCorner = currentPosition;


	deltaX = platform.GetMaxCorner().x - platform.GetMinCorner().x;
	deltaY = 0.0f;
		

	desiredNormal = glm::normalize(glm::vec2(-deltaY, deltaX));
		
	velocity = velocity - (glm::dot(velocity, desiredNormal)) * desiredNormal;
}

void Player::UpdatePhysics(Platform platforms[], int numCorners)
{
	oldPosition = currentPosition;

	//currentPosition.y -= velocity.y;

	for(int i = 0; i < numCorners; i++)
	{
		IsCollided(platforms[i]);/*
			
			currentPosition.y -= velocity.y;
			velocity.y = 0.0f;*/			
	}
}

void Player::UpdatePositions()
{
	glm::vec2 newCenter = currentPosition + glm::vec2(width / 2.0f, height / 2.0f);
	playerCollisionBody = CollisionBody_AABB_2D(newCenter, width / 2.0f, height / 2.0f);

	playerSprite.UpdateData(currentPosition);
}


void Player::Init()
{
	playerSprite.Init();
	playerSprite.LoadTexture("../data/player.png");
}

void Player::Update(Platform platforms[], int numCorners)
{
	if(fatCount < 3 || fatCount > 8)
	{
		velocity.x = 0.005f;
	}

	// На еди колко си цикъла, се сваля по 1 от fatCount

	velocity.y -= gravity;
	currentPosition.x -= platforms[0].GetLeftVelocity();
	currentPosition.y += velocity.y;
		
	UpdatePhysics(platforms, numCorners);
	UpdatePositions();
}

void Player::Render(GLuint shaderProgram)
{
	playerSprite.Draw(shaderProgram);
}

void Player::MoveLeft()
{
	oldPosition = currentPosition;

	currentPosition.x -= velocity.x;
}
void Player::MoveRight()
{
	oldPosition = currentPosition;

	currentPosition.x += velocity.x;
}

void Player::Jump(float units)
{
	if(!isJumping)
	{
		oldPosition = currentPosition;

		velocity.y += units;

		isJumping = true;
	}

	if(velocity.y == 0.0f)
	{
		isJumping = false;
	}
	else isJumping = true;
}