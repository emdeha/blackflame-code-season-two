#ifndef PLATFORM_H
#define PLATFORM_H


#include <glm/glm.hpp>

#include <glload/gl_3_3.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>


class Platform
{
private:
	glm::vec2 position;

	float width;
	float height;



	GLuint vertexBufferObject;
	GLuint vao;

	float vertexData[24];

public:
	Platform()
	{	
		position = glm::vec2();

		width = 0.0f;
		height = 0.0f;
	}

	Platform(glm::vec2 newPosition,
			 float newWidth, float newHeight)
	{
		position = newPosition;

		width = newWidth;
		height = newHeight;


		vertexData[0] = newPosition.x + newWidth;
		vertexData[1] = newPosition.y + newHeight;
		vertexData[2] = 0.0f; vertexData[3] = 1.0f;

		vertexData[4] = newPosition.x + newWidth;
		vertexData[5] = newPosition.y;
		vertexData[6] = 0.0f; vertexData[7] = 1.0f;

		vertexData[8] = newPosition.x;
		vertexData[9] = newPosition.y + newHeight;
		vertexData[10] = 0.0f; vertexData[11] = 1.0f;

		vertexData[12] = newPosition.x + newWidth;
		vertexData[13] = newPosition.y;
		vertexData[14] = 0.0f; vertexData[15] = 1.0f;

		vertexData[16] = newPosition.x;
		vertexData[17] = newPosition.y;
		vertexData[18] = 0.0f; vertexData[19] = 1.0f;

		vertexData[20] = newPosition.x;
		vertexData[21] = newPosition.y + newHeight;
		vertexData[22] = 0.0f; vertexData[23] = 1.0f;
	}

	void Init()
	{
		glGenBuffers(1, &vertexBufferObject);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	void Render(GLuint shaderProgram)
	{
		glUseProgram(shaderProgram);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}


	glm::vec2 GetMinCorner()
	{
		return position;
	}
	glm::vec2 GetMaxCorner()
	{
		return position + glm::vec2(width, height);
	}
};


#endif