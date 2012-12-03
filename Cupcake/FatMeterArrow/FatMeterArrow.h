#ifndef FAT_METER_ARROW_H
#define FAT_METER_ARROW_H


#include "../Sprite/Sprite.h"


class FatMeterArrow
{
private:
	glm::vec2 position;

	std::string textureName;

	Sprite arrowSprite;

public:
	FatMeterArrow() {}
	FatMeterArrow(glm::vec2 newPosition, const std::string &newTextureName);

	void Update(float newX);

	void Render(GLuint shaderProgram);
};

#endif