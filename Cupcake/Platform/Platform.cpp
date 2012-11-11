#include "stdafx.h"
#include "Platform.h"


Platform::Platform()
{	
	position = glm::vec2();

	leftMovementVelocity = 0.0f;

	width = 0.0f;
	height = 0.0f;

	isOutOfWindow = false;
}

Platform::Platform(glm::vec2 newPosition,
			       float newWidth, float newHeight,
				   float newLeftMovementVelocity)
{
	position = newPosition;

	leftMovementVelocity = newLeftMovementVelocity;

	width = newWidth;
	height = newHeight;

	isOutOfWindow = false;


	glm::vec2 collisionBodyCenter = position + glm::vec2(width / 2.0f, height / 2.0f);
	platformCollisionBody = CollisionBody_AABB_2D(collisionBodyCenter, width / 2.0f, height / 2.0f);


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

void Platform::Init()
{
	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void Platform::Update()
{
	position.x -= leftMovementVelocity;

	glm::vec2 newCenter = position + glm::vec2(width / 2.0f, height / 2.0f);
	platformCollisionBody = CollisionBody_AABB_2D(newCenter, width / 2.0f, height / 2.0f);

	glm::vec2 maxCorner = position + glm::vec2(width, height);
	if(maxCorner.x < -1.0f) isOutOfWindow = true;

	std::vector<float> newPositions(ARRAY_COUNT(vertexData));
	memcpy(&newPositions[0], vertexData, sizeof(vertexData));


	newPositions[0] = position.x + width;
	newPositions[1] = position.y + height;
	newPositions[2] = 0.0f; newPositions[3] = 1.0f;

	newPositions[4] = position.x + width;
	newPositions[5] = position.y;
	newPositions[6] = 0.0f; newPositions[7] = 1.0f;

	newPositions[8] = position.x;
	newPositions[9] = position.y + height;
	newPositions[10] = 0.0f; newPositions[11] = 1.0f;

	newPositions[12] = position.x + width;
	newPositions[13] = position.y;
	newPositions[14] = 0.0f; newPositions[15] = 1.0f;

	newPositions[16] = position.x;
	newPositions[17] = position.y;
	newPositions[18] = 0.0f; newPositions[19] = 1.0f;

	newPositions[20] = position.x;
	newPositions[21] = position.y + height;
	newPositions[22] = 0.0f; newPositions[23] = 1.0f;


	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), &newPositions[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Platform::Render(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);
		
	GLuint colorUnif = glGetUniformLocation(shaderProgram, "color");
	glUniform4f(colorUnif, fabsf(position.x), fabsf(position.y), 0.5f, 1.0f);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}