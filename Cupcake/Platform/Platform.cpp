#include "stdafx.h"
#include "Platform.h"


Platform::Platform()
{	
	position = glm::vec2();

	leftMovementVelocity = 0.0f;

	width = 0.0f;
	height = 0.0f;

	isOutOfWindow = false;
}

Platform::Platform(glm::vec2 newPosition,
			       float newWidth, float newHeight,
				   float newLeftMovementVelocity)
{
	position = newPosition;

	leftMovementVelocity = newLeftMovementVelocity;

	width = newWidth;
	height = newHeight;

	isOutOfWindow = false;


	glm::vec2 collisionBodyCenter = position + glm::vec2(width / 2.0f, height / 2.0f);
	platformCollisionBody = CollisionBody_AABB_2D(collisionBodyCenter, width / 2.0f, height / 2.0f);

	platformSprite = Sprite(position, glm::vec4(0.5f, 0.3f, 1.0f, 1.0f), width, height);
}

void Platform::Init()
{
	platformSprite.Init();
	platformSprite.LoadTexture("../data/platform.png");
}

void Platform::Update()
{
	position.x -= leftMovementVelocity;

	glm::vec2 newCenter = position + glm::vec2(width / 2.0f, height / 2.0f);
	platformCollisionBody = CollisionBody_AABB_2D(newCenter, width / 2.0f, height / 2.0f);

	glm::vec2 maxCorner = position + glm::vec2(width, height);
	if(maxCorner.x < -1.0f) isOutOfWindow = true;


	platformSprite.UpdateData(position);
}

void Platform::Render(GLuint shaderProgram)
{
	platformSprite.Draw(shaderProgram);
}