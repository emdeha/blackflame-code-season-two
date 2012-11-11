#include <ctime>

#include "Main.h"
#include "../Platform/Platform.h"
#include "../Player/Player.h"
#include "../Cake/Cake.h"

#include "../Mouse/Mouse.h"


// TODO: When the player jumps upwards, he can stick to the platform's bottom and move.


Mouse userMouse;


GLuint theProgram;
GLuint offsetUniform;
GLuint isPlayerUniform;


const float JUMP_UNITS = 0.0005f;

const float PLATFORMS_WIDTH = 0.4f;
const float PLATFORMS_HEIGHT = 0.15f;
const int PLATFORMS_COUNT = 10;
const float PLATFORMS_LEFT_VELOCITY = 0.0001f;

Platform platforms[PLATFORMS_COUNT];
Player player;

const int GRID_WIDTH = (int)(2.0f / 0.4f);
const int GRID_HEIGHT = (int)(2.0f / 0.15f);

struct GridCell
{
	bool isTaken;
	glm::vec2 position;
};

GridCell platformsGrid[GRID_WIDTH][GRID_HEIGHT];

void FillPlatformsGrid()
{
	for(int col = 0; col < GRID_WIDTH; col++)
	{
		for(int row = 0; row < GRID_HEIGHT; row++)
		{
			platformsGrid[col][row].isTaken = false;
		}
	}
	
	float posX = -1.0f;
	float posY = -1.0f;
	for(int col = 0; col < GRID_WIDTH; col++)
	{
		for(int row = 0; row < GRID_HEIGHT; row++)
		{
			platformsGrid[col][row].position = glm::vec2(posX, posY);
			posY += PLATFORMS_HEIGHT;
		}
		posY = -1.0f;
		posX += PLATFORMS_WIDTH;
	}
}

bool IsCollided(Platform firstPlatform, Platform secondPlatform)
{
	glm::vec2 firstPlatformCenterPoint = 
		glm::vec2(firstPlatform.GetMinCorner().x + firstPlatform.GetWidth() / 2.0f, 
				  firstPlatform.GetMinCorner().y + firstPlatform.GetHeight() / 2.0f);
	glm::vec2 secondPlatformCenterPoint = 
		glm::vec2(secondPlatform.GetMinCorner().x + secondPlatform.GetWidth() / 2.0f,
				  secondPlatform.GetMinCorner().y + secondPlatform.GetHeight() / 2.0f);


	glm::vec2 firstPlatformHalfWidths = glm::vec2(firstPlatform.GetWidth() / 2.0f, firstPlatform.GetHeight() / 2.0f);
	glm::vec2 secondPlatformHalfWidths = glm::vec2(secondPlatform.GetWidth() / 2.0f, secondPlatform.GetHeight() / 2.0f);


	if(fabsf(firstPlatformCenterPoint.x - secondPlatformCenterPoint.x) >=
	   (firstPlatformHalfWidths.x + secondPlatformHalfWidths.x))
		return false;
	if(fabsf(firstPlatformCenterPoint.y - secondPlatformCenterPoint.y) >=
	   (firstPlatformHalfWidths.y + secondPlatformHalfWidths.y))
		return false;

	return true;
}
bool IsCollided(Cake cake, Player player)
{
	glm::vec2 cakeCenterPoint = 
		glm::vec2(cake.GetMinCorner().x + cake.GetWidth() / 2.0f, 
				  cake.GetMinCorner().y + cake.GetHeight() / 2.0f);
	glm::vec2 playerCenterPoint = 
		glm::vec2(player.GetMinCorner().x + player.GetWidth() / 2.0f,
				  player.GetMinCorner().y + player.GetHeight() / 2.0f);


	glm::vec2 cakeHalfWidths = glm::vec2(cake.GetWidth() / 2.0f, cake.GetHeight() / 2.0f);
	glm::vec2 playerHalfWidths = glm::vec2(player.GetWidth() / 2.0f, player.GetHeight() / 2.0f);


	if(fabsf(cakeCenterPoint.x - playerCenterPoint.x) >
		(cakeHalfWidths.x + playerHalfWidths.x))
		return false;
	if(fabsf(cakeCenterPoint.y - playerCenterPoint.y) >
		(cakeHalfWidths.y + playerHalfWidths.y))
		return false;


	return true;
}


void RandomizePlatforms(Platform platformArray[], int platformCount)
{
	srand((unsigned) time(0));

	const int NUM_PLATFORMS = platformCount;

	FillPlatformsGrid();

	for(int i = 0; i < platformCount; i++)
	{
		glm::vec2 platformPos;

		int gridCol = rand() % (GRID_WIDTH);
		int	gridRow = rand() % (GRID_HEIGHT);
		platformPos = platformsGrid[gridCol][gridRow].position;

		while(platformsGrid[gridCol][gridRow].isTaken)
		{
			gridCol = rand() % (GRID_WIDTH);
			gridRow = rand() % (GRID_HEIGHT);
			platformPos = platformsGrid[gridCol][gridRow].position;
		}
		

		platformsGrid[gridCol][gridRow].isTaken = true;

		platformArray[i] = Platform(platformPos,
									PLATFORMS_WIDTH, PLATFORMS_HEIGHT,
									PLATFORMS_LEFT_VELOCITY);
	}

	/*
	for(int currPlatform = 0; currPlatform < platformCount; currPlatform++)
	{
		glm::vec2 newPlatformPosition;
		newPlatformPosition.x = 2 * ((float) rand() / RAND_MAX) - 1;
		newPlatformPosition.y = 2 * ((float) rand() / RAND_MAX) - 1;
		
		platformArray[currPlatform] =
			Platform(newPlatformPosition,
					 PLATFORMS_WIDTH, PLATFORMS_HEIGHT,
					 PLATFORMS_LEFT_VELOCITY);

		for(int i = 0; i < platformCount; i++)
		{
			if(i != currPlatform)
			{
				while(IsCollided(platforms[currPlatform], platforms[i]))
				{
					newPlatformPosition.x = 2 * ((float) rand() / RAND_MAX) - 1;
					newPlatformPosition.y = 2 * ((float) rand() / RAND_MAX) - 1;

					platformArray[currPlatform] =
						Platform(newPlatformPosition,
								 PLATFORMS_WIDTH, PLATFORMS_HEIGHT,
								 PLATFORMS_LEFT_VELOCITY);
				}
			}
		}
	}
	*/
}


void HandleMouse()
{
	if(userMouse.IsLeftButtonDown())
	{
		player.Jump(JUMP_UNITS);
	}
}
void HandleMouseButtons(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		userMouse.PressLeftButton();
	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		userMouse.ReleaseLeftButton();
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		userMouse.PressRightButton();
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		userMouse.ReleaseRightButton();
	}
}
void HandleActiveMovement(int x, int y)
{
	userMouse.SetCurrentPosition(glm::ivec2(x, y));
}
void HandlePassiveMovement(int x, int y)
{
	userMouse.SetCurrentPosition(glm::ivec2(x, y));
}


void InitializeShaders(const std::string &vertexShader, 
					   const std::string &fragmentShader)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, vertexShader));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, fragmentShader));

	theProgram = Framework::CreateProgram(shaderList);
}


const float vertexData[] =
{
	+0.5f, +0.5f, 0.0f, 1.0f,
	+0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, +0.5f, 0.0f, 1.0f,

	+0.5f, -0.5f, 0.0f, 1.0f, 
	-0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, +0.5f, 0.0f, 1.0f,
};


GLuint vertexBufferObject;
GLuint vao;


void InitializeVertexBuffer()
{
	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


	
glm::vec2 platformsMinCorners[PLATFORMS_COUNT];
glm::vec2 platformsMaxCorners[PLATFORMS_COUNT];

Cake testCake;

void InitObjects()
{
	RandomizePlatforms(platforms, PLATFORMS_COUNT);

	for(int currPlatform = 0; currPlatform < PLATFORMS_COUNT; currPlatform++)
	{
		platforms[currPlatform].Init();
		platformsMinCorners[currPlatform] = platforms[currPlatform].GetMinCorner();
		platformsMaxCorners[currPlatform] = platforms[currPlatform].GetMaxCorner();
	}
	srand((unsigned) time(0));

	int platformIndex = rand() % PLATFORMS_COUNT;

	float cakePosY = platforms[platformIndex].GetMaxCorner().y;
	float cakePosX = platforms[platformIndex].GetMaxCorner().x - platforms[platformIndex].GetWidth() / 2.0f;

	testCake = Cake(glm::vec2(cakePosX, cakePosY), 5, 0.1f, 0.1f);
	testCake.Init();

	glm::vec2 playerPos = platforms[0].GetMaxCorner();

	player = Player(playerPos, glm::vec2(0.01f, 0.0f),
					0.05f, 0.1f, 5);

	player.Init();
}

void Init()
{
	InitializeShaders("VertexShader.vert", "FragmentShader.frag");

	InitObjects();
	
	InitializeVertexBuffer();


	glutMouseFunc(HandleMouseButtons);
	glutMotionFunc(HandleActiveMovement);
	glutPassiveMotionFunc(HandlePassiveMovement);


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

float _x = 0.0f;
float _y = 0.0f;

void Display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for(int i = 0; i < PLATFORMS_COUNT; i++)
	{
		if(platforms[i].IsOutOfWindow())
		{
			for(int col = 0; col < GRID_WIDTH; col++)
			{
				for(int row = 0; row < GRID_HEIGHT; row++)
				{
					if(platformsGrid[col][row].position == platforms[i].GetMinCorner())
					{
						platformsGrid[col][row].isTaken = false;
					}
				}
			}

			glm::vec2 platformPos;

			int gridCol = rand() % (GRID_WIDTH);
			int	gridRow = rand() % (GRID_HEIGHT);
			platformPos = platformsGrid[gridCol][gridRow].position;

			while(platformsGrid[gridCol][gridRow].isTaken)
			{
				gridCol = rand() % (GRID_WIDTH);
				gridRow = rand() % (GRID_HEIGHT);
				platformPos = platformsGrid[gridCol][gridRow].position;
			}
		

			platformsGrid[gridCol][gridRow].isTaken = true;

			platforms[i] = Platform(platformPos,
										PLATFORMS_WIDTH, PLATFORMS_HEIGHT,
										PLATFORMS_LEFT_VELOCITY);
			platforms[i].Init();
		}

		platforms[i].Update();
		platforms[i].Render(theProgram);
	}
	/*for(int currPlatform = 0; currPlatform < PLATFORMS_COUNT; currPlatform++)
	{
		if(platforms[currPlatform].IsOutOfWindow())
		{
			glm::vec2 newPlatformPosition;
			newPlatformPosition.x = 2 * ((float) rand() / RAND_MAX) + 0.5;
			newPlatformPosition.y = 2 * ((float) rand() / RAND_MAX) - 1;
		
			platforms[currPlatform] =
				Platform(newPlatformPosition,
						 PLATFORMS_WIDTH, PLATFORMS_HEIGHT,
						 PLATFORMS_LEFT_VELOCITY);

			for(int i = 0; i < PLATFORMS_COUNT; i++)
			{
				if(i != currPlatform)
				{
					while(IsCollided(platforms[currPlatform], platforms[i]))
					{
						newPlatformPosition.x = 2 * ((float) rand() / RAND_MAX) + 0.5;
						newPlatformPosition.y = 2 * ((float) rand() / RAND_MAX) - 1;

						platforms[currPlatform] =
							Platform(newPlatformPosition,
									 PLATFORMS_WIDTH, PLATFORMS_HEIGHT,
									 PLATFORMS_LEFT_VELOCITY);
					}
				}
			}

			platforms[currPlatform].Init();
		}*/

	//}

	if(IsCollided(testCake, player))
	{
		player.AddFat(testCake.GetFat());
		testCake.Eat();
	}

	testCake.Update(PLATFORMS_LEFT_VELOCITY);
	testCake.Render(theProgram);

	player.Update(platforms, PLATFORMS_COUNT);

	player.Render(theProgram);

	/*
	glUseProgram(theProgram);

	//glUniform2f(offsetUniform, _x, _y);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUseProgram(0);
	*/

	HandleMouse();

	glutSwapBuffers();
	glutPostRedisplay();
}
void Reshape(int width, int height)
{
	//glViewport(0, 0, width, height);
}


void Keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:
		glutLeaveMainLoop();
		return;

	case 'a':
		player.MoveLeft();
		break;

	case 'd':
		player.MoveRight();
		break;
	}
}

unsigned int Defaults(unsigned int displayMode, int &width, int &height)
{
	return displayMode;
}