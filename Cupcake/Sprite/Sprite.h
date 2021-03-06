#ifndef SPRITE_H
#define SPRITE_H


#include "../glfiles/gl-headers/glload/gl_3_3.h"
#include "../glfiles/gl-headers/glload/gll.hpp"

#include "../glfiles/gl-headers/glm/glm.hpp"

#include <memory>
#include <map>

#include "Texture2D.h"


class Sprite
{
private:
	glm::vec2 position;

	glm::vec4 color;

	float width;
	float height;


	GLuint vertexBO;
	GLuint indexBO;
	GLuint textureCoordsBO;
	GLuint vao;


	std::shared_ptr<Texture2D> currentTexture;
	std::map<std::string, std::shared_ptr<Texture2D>> texture;

public:
	Sprite() {}
	Sprite(glm::vec2 newPosition, glm::vec4 newColor, 
		   float newWidth, float newHeight);

	void Init();

	void AddTexture(const std::string &imageFileName);
	//void LoadTexture(const std::string &imageFileName);

	void UpdateData(glm::vec2 newPosition);

	void Draw(GLuint shaderProgram);

	void ChangeTexture(const std::string &imageFileName);
};


#endif