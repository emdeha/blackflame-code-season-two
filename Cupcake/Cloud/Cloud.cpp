#include "stdafx.h"
#include "Cloud.h"


#define WIDTH 0.3f
#define HEIGHT 0.3f


Cloud::Cloud(float positionX, float newLayerPosition, 
			 const std::string &spriteTexture)
{
	layerPosition = newLayerPosition;

	position = glm::vec2(positionX, newLayerPosition);

	cloudSprite = Sprite(position, glm::vec4(), WIDTH, HEIGHT);
	cloudSprite.Init();
	cloudSprite.AddTexture(spriteTexture);
	cloudSprite.ChangeTexture(spriteTexture);
}

void Cloud::Update(float leftVelocity)
{
	if(position.x + WIDTH <= -1.0f)
	{
		position.x = (float) rand() / RAND_MAX + 1.0f;
	}

	position.x -= leftVelocity;

	cloudSprite.UpdateData(position);
}

void Cloud::Render(GLuint shaderProgram)
{
	cloudSprite.Draw(shaderProgram);
}