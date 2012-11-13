#ifndef SPRITE_H
#define SPRITE_H


#include <glload/gl_3_3.h>
#include <glload/gll.hpp>

#include <glm/glm.hpp>


class Sprite
{
private:
	glm::vec2 position;

	glm::vec4 color;

	float width;
	float height;


	GLuint vertexBO;
	GLuint vao;

public:
	Sprite() {}
	Sprite(glm::vec2 newPosition, glm::vec4 newColor, 
		   float newWidth, float newHeight);

	void Init();

	void Draw(GLuint shaderProgram);
};


#endif