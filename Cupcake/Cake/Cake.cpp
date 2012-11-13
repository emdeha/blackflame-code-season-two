#include "stdafx.h"
#include "Cake.h"

#include "../Framework/Framework.h"

#include <vector>


Cake::Cake()
{
	position = glm::vec2();
	fatPoints = 0;
	isEaten = false;
	isOutOfWindow = false;
}
Cake::Cake(glm::vec2 newPosition, float newFatPoints,
		   float newWidth, float newHeight)
{
	position = newPosition;
	fatPoints = newFatPoints;
	isEaten = false;

	width = newWidth;
	height = newHeight;
	isOutOfWindow = false;

	
	cakeSprite = Sprite(position, glm::vec4(1.0f, 0.3f, 0.5f, 1.0f), width, height);
}

void Cake::Init()
{
	cakeSprite.Init();
	cakeSprite.LoadTexture("../data/cake.png");
}

void Cake::Update(float leftVelocity)
{
	if(!isEaten)
	{
		position.x -= leftVelocity;
		
		glm::vec2 newCenter = position + glm::vec2(width / 2.0f, height / 2.0f);
		cakeCollisionBody = CollisionBody_AABB_2D(newCenter, width / 2.0f, height / 2.0f);

		glm::vec2 maxCorner = position + glm::vec2(width, height);
		if(maxCorner.x < -1.0f) isOutOfWindow = true;


		cakeSprite.UpdateData(position);
	}
}


void Cake::Render(GLuint shaderProgram)
{
	if(!isEaten)
	{
		cakeSprite.Draw(shaderProgram);
	}
}