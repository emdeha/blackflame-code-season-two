#include "stdafx.h"
#include "Cake.h"


#include <vector>


Cake::Cake()
{
	position = glm::vec2();
	fatPoints = 0;
	isEaten = false;
	isOutOfWindow = false;
}
Cake::Cake(glm::vec2 newPosition, 
		   float newAnimationMaxOffset,
		   float newAnimationOffset,
		   float newFatPoints,
		   float newWidth, float newHeight)
{
	position = newPosition;
	fatPoints = newFatPoints;
	isEaten = false;

	width = newWidth;
	height = newHeight;
	isOutOfWindow = false;


	animationMaxY = newAnimationMaxOffset + position.y;
	animationMinY = position.y;
	animationOffset = newAnimationOffset;
	
	cakeSprite = Sprite(position, glm::vec4(1.0f, 0.3f, 0.5f, 1.0f), width, height);
}

void Cake::Init()
{
	cakeSprite.Init();
	cakeSprite.AddTexture("../data/cake.png");
	cakeSprite.ChangeTexture("../data/cake.png");
}

void Cake::Update(float leftVelocity)
{
	if(!isEaten)
	{
		position.x -= leftVelocity;

		if(position.y < animationMinY)
		{
			animationOffset = animationOffset * (-1.0f);
		}
		if(position.y > animationMaxY)
		{
			animationOffset = animationOffset * (-1.0f);
		}
		
		position.y += animationOffset;
				
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