#include "stdafx.h"
#include "FatMeterArrow.h"


FatMeterArrow::FatMeterArrow(glm::vec2 newPosition, const std::string &newTextureName)
{
	position = newPosition;
	textureName = newTextureName;

	arrowSprite = Sprite(position, glm::vec4(), 0.01f, 0.1f);
	arrowSprite.Init();
	arrowSprite.AddTexture(textureName);
	arrowSprite.ChangeTexture(textureName);
}

void FatMeterArrow::Update(float newX)
{
	glm::vec2 newPosition = position;
	newPosition.x = position.x + (newX / 10.0f);

	//position.x = (newX / 10.0f);
	arrowSprite.UpdateData(newPosition);
}

void FatMeterArrow::Render(GLuint shaderProgram)
{
	arrowSprite.Draw(shaderProgram);
}