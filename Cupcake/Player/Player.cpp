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


	vertexData[0] = newPosition.x + newWidth;
	vertexData[1] = newPosition.y + newHeight;
	vertexData[2] = 0.0f; vertexData[3] = 1.0f;

	vertexData[4] = newPosition.x + newWidth;
	vertexData[5] = newPosition.y;
	vertexData[6] = 0.0f; vertexData[7] = 1.0f;

	vertexData[8] = newPosition.x;
	vertexData[9] = newPosition.y + newHeight;
	vertexData[10] = 0.0f; vertexData[11] = 1.0f;

	vertexData[12] = newPosition.x + newWidth;
	vertexData[13] = newPosition.y;
	vertexData[14] = 0.0f; vertexData[15] = 1.0f;

	vertexData[16] = newPosition.x;
	vertexData[17] = newPosition.y;
	vertexData[18] = 0.0f; vertexData[19] = 1.0f;

	vertexData[20] = newPosition.x;
	vertexData[21] = newPosition.y + newHeight;
	vertexData[22] = 0.0f; vertexData[23] = 1.0f;
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

	std::vector<float> newPositions(ARRAY_COUNT(vertexData));
	memcpy(&newPositions[0], vertexData, sizeof(vertexData));


	newPositions[0] = currentPosition.x + width;
	newPositions[1] = currentPosition.y + height;
	newPositions[2] = 0.0f; newPositions[3] = 1.0f;

	newPositions[4] = currentPosition.x + width;
	newPositions[5] = currentPosition.y;
	newPositions[6] = 0.0f; newPositions[7] = 1.0f;

	newPositions[8] = currentPosition.x;
	newPositions[9] = currentPosition.y + height;
	newPositions[10] = 0.0f; newPositions[11] = 1.0f;

	newPositions[12] = currentPosition.x + width;
	newPositions[13] = currentPosition.y;
	newPositions[14] = 0.0f; newPositions[15] = 1.0f;

	newPositions[16] = currentPosition.x;
	newPositions[17] = currentPosition.y;
	newPositions[18] = 0.0f; newPositions[19] = 1.0f;

	newPositions[20] = currentPosition.x;
	newPositions[21] = currentPosition.y + height;
	newPositions[22] = 0.0f; newPositions[23] = 1.0f;


	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), &newPositions[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Player::Init()
{
	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
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
	glUseProgram(shaderProgram);

	GLuint colorUnif = glGetUniformLocation(shaderProgram, "color");
	glUniform4f(colorUnif, 0.0f, 1.0f, 0.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
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