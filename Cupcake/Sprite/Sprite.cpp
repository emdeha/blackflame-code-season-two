#include "stdafx.h"
#include "Sprite.h"


#include <vector>


Sprite::Sprite(glm::vec2 newPosition, glm::vec4 newColor, 
			   float newWidth, float newHeight)
{
	position = newPosition;
	
	color = newColor;

	width = newWidth;
	height = newHeight;

	vertexBO = 0;
	vao = 0;
}

void Sprite::Init()
{
	std::vector<float> vertexData;

	vertexData.push_back(position.x + width);
	vertexData.push_back(position.y + height);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(position.x + width);
	vertexData.push_back(position.y);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(position.x);
	vertexData.push_back(position.y + height);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(position.x + width);
	vertexData.push_back(position.y);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(position.x);
	vertexData.push_back(position.y);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(position.x);
	vertexData.push_back(position.y + height);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);


	glGenBuffers(1, &vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
	glBufferData(GL_ARRAY_BUFFER, 
				 sizeof(float) * vertexData.size(), 
				 &vertexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}


void Sprite::Draw(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);

	GLuint colorUnif = glGetUniformLocation(shaderProgram, "color");
	glUniform4f(colorUnif, color.r, color.g, color.b, color.a);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}