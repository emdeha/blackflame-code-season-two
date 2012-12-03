#ifndef CLOUD_H
#define CLOUD_H


#include "../Sprite/Sprite.h"


class Cloud
{
private:
	glm::vec2 position;

	float layerPosition;

	Sprite cloudSprite;

public:
	Cloud() {}
	Cloud(float positionX, float newLayerPosition,
		  const std::string &spriteTexture);

	void Update(float leftVelocity);
	void Render(GLuint shaderProgram);
};


#endif