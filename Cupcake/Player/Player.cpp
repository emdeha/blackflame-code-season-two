#include "stdafx.h"
#include "Player.h"


const float INITIAL_VELOCITY_X = 0.01f;
const float INITIAL_JUMP_UNITS = 0.0015f;


Player::Player()
{
	currentPosition = glm::vec2();
	velocity = glm::vec2();
	offset = glm::vec2();

	width = 0.0f;
	height = 0.0f;

	isJumping = false;

	fatCount = 0;

	playerCollisionBody = CollisionBody_AABB_2D(glm::vec2(), 0.0f, 0.0f);
}
Player::Player(glm::vec2 newPosition, glm::vec2 newVelocity,
			   double newStarvingTime_secs,
			   float newJumpUnits,
			   float newWidth, float newHeight)
{
	currentPosition = newPosition;
	velocity = newVelocity;

	offset = glm::vec2();

	width = newWidth;
	height = newHeight;

	starvingTime_secs = newStarvingTime_secs;
	lastTime = clock();

	isJumping = false;
	isDead = false;

	jumpUnits = newJumpUnits;

	fatCount = 1.0f;

	glm::vec2 collisionBodyCenter = currentPosition + glm::vec2(width / 2.0f, height / 2.0f);
	playerCollisionBody = CollisionBody_AABB_2D(collisionBodyCenter, width / 2.0f, height / 2.0f);


	playerSprite = Sprite(currentPosition, glm::vec4(0.3f, 1.0f, 0.5f, 1.0f), width, height);
}


void Player::IsCollided(Platform &platform)
{
	if(!platform.GetCollisionBody().IsCollided(playerCollisionBody))
		return;
		
	//velocity.y += gravity;

	float deltaX = 0.0f;
	float deltaY = 0.0f;

	glm::vec2 desiredNormal;

	glm::vec2 playerMaxCorner = currentPosition + glm::vec2(width, height);
	glm::vec2 playerMinCorner = currentPosition;


	deltaX = platform.GetMaxCorner().x - platform.GetMinCorner().x;
	deltaY = 0.0f;
		

	desiredNormal = glm::normalize(glm::vec2(-deltaY, deltaX));
	

	if(playerMinCorner.y + 0.01f >= platform.GetMaxCorner().y)
	{
		velocity = velocity - (glm::dot(velocity, desiredNormal)) * desiredNormal;
	}
}

void Player::UpdatePhysics(Platform platforms[], int numCorners)
{	
	velocity.y -= gravity;

	currentPosition.x -= platforms[0].GetLeftVelocity();
	currentPosition.y += velocity.y;
	oldPosition = currentPosition;
	/*
	if(currentPosition.x < -1.0f || currentPosition.y < -1.0f)
	{
		isDead = true;
	}
	*/
	//currentPosition.y -= velocity.y;

	for(int i = 0; i < numCorners; i++)
	{
		IsCollided(platforms[i]);/*
			
			currentPosition.y -= velocity.y;
			velocity.y = 0.0f;*/			
	}
}

void Player::UpdatePositions()
{
	glm::vec2 newCenter = currentPosition + glm::vec2(width / 2.0f, height / 2.0f);
	playerCollisionBody = CollisionBody_AABB_2D(newCenter, width / 2.0f, height / 2.0f);

	playerSprite.UpdateData(currentPosition);
}


void Player::Init()
{
	playerSprite.Init();
	playerSprite.AddTexture("../data/player-idle.png");
	playerSprite.AddTexture("../data/player-move.png");
	playerSprite.AddTexture("../data/player-jump.png");
	playerSprite.ChangeTexture("../data/player-idle.png");
}

void Player::Update(Platform platforms[], int numCorners)
{
	//if(!isDead)
	//{
		// На еди колко си цикъла, се сваля по 1 от fatCount
		/*if(fatCount <= 0.9f || fatCount >= 1.1f)
		{
			velocity.x -= 0.000005f;
			jumpUnits -= 0.00000005f;
			*/
			if(velocity.x <= 0.0f)
			{
				isDead = true;
			}
		//}
			
		clock_t currentTime = clock();
		double elapsedSeconds = double(currentTime - lastTime) / CLOCKS_PER_SEC;
		if(elapsedSeconds >= starvingTime_secs)
		{			
			lastTime = clock();

			if(fatCount <= 0.9f || fatCount >= 1.1f)
			{
				velocity.x -= 0.00005f;
				jumpUnits -= 0.0000005f;
			}
			else
			{
				if(velocity.x <= INITIAL_VELOCITY_X)
				{
					velocity.x += 0.00005f;
				}
				if(jumpUnits <= INITIAL_JUMP_UNITS)
				{ 
					jumpUnits += 0.0000005f;
				}
			}
			fatCount -= 0.05f;
		}

		
		UpdatePhysics(platforms, numCorners);
		UpdatePositions();

		//playerSprite.ChangeTexture("../data/player-idle.png");
	//}
}

void Player::Render(GLuint shaderProgram)
{
	//if(!isDead)
	//{
		playerSprite.Draw(shaderProgram);
	//}
}

void Player::MoveLeft()
{
	//if(!isDead)
	//{
		playerSprite.ChangeTexture("../data/player-move.png");	

		oldPosition = currentPosition;

		currentPosition.x -= velocity.x;
	//}
}
void Player::MoveRight()
{
	//if(!isDead)
	//{
		playerSprite.ChangeTexture("../data/player-move.png");

		oldPosition = currentPosition;

		currentPosition.x += velocity.x;
	//}
}

void Player::Jump()
{
	//if(!isDead)
	//{
		playerSprite.ChangeTexture("../data/player-jump.png");

		if(!isJumping)
		{
			oldPosition = currentPosition;

			velocity.y += jumpUnits;

			isJumping = true;
		}

		if(velocity.y == 0.0f)
		{
			isJumping = false;
		}
		else isJumping = true;
	//}
}