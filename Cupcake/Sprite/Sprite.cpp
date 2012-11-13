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
	indexBO = 0;
	textureCoordsBO = 0;
	vao = 0;


	texture = std::shared_ptr<Texture2D>(new Texture2D());
}

void Sprite::Init()
{
	std::vector<float> vertexData;
	std::vector<float> texCoordsData;
	std::vector<unsigned short> indexData;
	

	vertexData.push_back(position.x);
	vertexData.push_back(position.y + height);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(position.x + width);
	vertexData.push_back(position.y + height);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(position.x + width);
	vertexData.push_back(position.y);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(position.x);
	vertexData.push_back(position.y);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);


	indexData.push_back(0); indexData.push_back(1); indexData.push_back(2);
	indexData.push_back(2); indexData.push_back(3); indexData.push_back(0);


	texCoordsData.push_back(0.0f); texCoordsData.push_back(1.0f);
	texCoordsData.push_back(1.0f); texCoordsData.push_back(1.0f);
	texCoordsData.push_back(1.0f); texCoordsData.push_back(0.0f);
	texCoordsData.push_back(0.0f); texCoordsData.push_back(0.0f);


	glGenBuffers(1, &vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
	glBufferData(GL_ARRAY_BUFFER, 
				 sizeof(float) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &textureCoordsBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordsBO);
	glBufferData(GL_ARRAY_BUFFER, 
				 sizeof(float) * texCoordsData.size(), &texCoordsData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 sizeof(unsigned short) * indexData.size(), &indexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}


void Sprite::LoadTexture(const std::string &imageFileName)
{
	if(!texture->Load(imageFileName))
	{
		std::printf("Error loading texture");
		return;
	}
}


void Sprite::UpdateData(glm::vec2 newPosition)
{
	std::vector<float> vertexData;

	vertexData.push_back(newPosition.x);
	vertexData.push_back(newPosition.y + height);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(newPosition.x + width);
	vertexData.push_back(newPosition.y + height);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(newPosition.x + width);
	vertexData.push_back(newPosition.y);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);

	vertexData.push_back(newPosition.x);
	vertexData.push_back(newPosition.y);
	vertexData.push_back(0.0f); vertexData.push_back(1.0f);


	glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), &vertexData[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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


	glBindBuffer(GL_ARRAY_BUFFER, textureCoordsBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);

	texture->Bind(GL_TEXTURE0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}