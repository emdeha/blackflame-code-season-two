#ifndef PLAYER_H
#define PLAYER_H


#include <glm/glm.hpp>

#include <glload/gl_3_3.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>


#include "../Framework/Framework.h"


#include <vector>


static float gravity = 0.0000005f;


class Player
{
private:
	glm::vec2 currentPosition;
	glm::vec2 oldPosition;

	glm::vec2 velocity;

	glm::vec2 offset;

	
	float width;
	float height;


	GLuint vertexBufferObject;
	GLuint vao;

	float vertexData[24];


	bool isJumping;


	void IsCollided(glm::vec2 minCorners[], glm::vec2 maxCorners[], 
					int numCorners)
	{
		glm::vec2 playerMinCorner = currentPosition;
		glm::vec2 playerMaxCorner = currentPosition + glm::vec2(width, height);


		for(int corner = 0; corner < numCorners; corner++)
		{
			if(playerMinCorner.x >= minCorners[corner].x &&
			   playerMinCorner.y >= minCorners[corner].y &&
			   playerMinCorner.x <= maxCorners[corner].x &&
			   playerMinCorner.y <= maxCorners[corner].y)
			{
				currentPosition.y -= velocity.y;
				velocity.y = 0.0f;
				return;
			}			

			if(playerMaxCorner.x >= minCorners[corner].x &&
			   playerMaxCorner.y >= minCorners[corner].y &&
			   playerMaxCorner.x <= maxCorners[corner].x &&
			   playerMaxCorner.y <= maxCorners[corner].y)
			{
				currentPosition.y += velocity.y;
				velocity.y = 0.0f;
				return;
			}
		}
	}

	void UpdatePhysics(glm::vec2 minCorners[], glm::vec2 maxCorners[], 
					   int numCorners)
	{
		oldPosition = currentPosition;

		//currentPosition.y -= velocity.y;

		IsCollided(minCorners, maxCorners, numCorners);
	}
	void UpdatePositions()
	{
		std::vector<float> newPositions(ARRAY_COUNT(vertexData));
		memcpy(&newPositions[0], vertexData, sizeof(vertexData));


		newPositions[0] = currentPosition.x + width;
		newPositions[1] = currentPosition.y + height;
		newPositions[2] = 0.0f; newPositions[3] = 1.0f;

		newPositions[4] = currentPosition.x + width;
		newPositions[5] = currentPosition.y;
		newPositions[6] = 0.0f; newPositions[7] = 1.0f;

		newPositions[8] = currentPosition.x;
		newPositions[9] = currentPosition.y + height;
		newPositions[10] = 0.0f; newPositions[11] = 1.0f;

		newPositions[12] = currentPosition.x + width;
		newPositions[13] = currentPosition.y;
		newPositions[14] = 0.0f; newPositions[15] = 1.0f;

		newPositions[16] = currentPosition.x;
		newPositions[17] = currentPosition.y;
		newPositions[18] = 0.0f; newPositions[19] = 1.0f;

		newPositions[20] = currentPosition.x;
		newPositions[21] = currentPosition.y + height;
		newPositions[22] = 0.0f; newPositions[23] = 1.0f;
		/*for(int vertex = 0; vertex < ARRAY_COUNT(vertexData); vertex += 4)
		{
			newPositions[vertex] = currentPosition.x;
			newPositions[vertex + 1] = currentPosition.y;
		}*/


		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), &newPositions[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

public:
	Player()
	{
		currentPosition = glm::vec2();
		velocity = glm::vec2();
		offset = glm::vec2();

		width = 0.0f;
		height = 0.0f;

		isJumping = false;
	}
	Player(glm::vec2 newPosition, glm::vec2 newVelocity,
		   float newWidth, float newHeight)
	{
		currentPosition = newPosition;
		velocity = newVelocity;

		offset = glm::vec2();

		width = newWidth;
		height = newHeight;

		isJumping = false;


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

	void Update(glm::vec2 minCorners[], glm::vec2 maxCorners[], 
				int numCorners)
	{
		velocity.y -= gravity;
		currentPosition.y += velocity.y;
		
		UpdatePhysics(minCorners, maxCorners, numCorners);
		UpdatePositions();
	}

	void Render(GLuint shaderProgram)
	{
		glUseProgram(shaderProgram);
		

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}

	void MoveLeft()
	{
		oldPosition = currentPosition;

		currentPosition.x -= velocity.x;
		//offset.x -= units;
	}
	void MoveRight()
	{
		oldPosition = currentPosition;

		currentPosition.x += velocity.x;
		//offset.x += units;
	}

	void Jump(float units)
	{
		if(!isJumping)
		{
			oldPosition = currentPosition;

			velocity.y += units;

			isJumping = true;
		}

		if(velocity.y == 0.0f)
		{
			isJumping = false;
		}
		else isJumping = true;
	}
};


#endif