#include <ctime>

#include "Main.h"
#include "../Platform/Platform.h"
#include "../Player/Player.h"
#include "../Cake/Cake.h"
#include "../FatMeterArrow/FatMeterArrow.h"
#include "../Cloud/Cloud.h"

#include "../Mouse/Mouse.h"


// TODO: When the player jumps upwards, he can stick to the platform's bottom and move.


Mouse userMouse;


FatMeterArrow fatMeterArrow;
Sprite fatMeter;
Sprite jumpArrow;
Sprite jumpArrowRight;
Sprite jumpArrowDiag;


GLuint theProgram;
GLuint offsetUniform;
GLuint isPlayerUniform;



const int TICKS_PER_SECOND = 50;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 10;



Cloud clouds[6];

#ifdef _DEBUG
const float JUMP_UNITS = 0.0015f;
const float PLATFORMS_LEFT_VELOCITY = 0.0002f;
const float CLOUD_LEFT_VELOCITY = 0.0001f;
#elif defined RELEASE
const float JUMP_UNITS = 0.0015f;
const float PLATFORMS_LEFT_VELOCITY = 0.0001f;
const float CLOUD_LEFT_VELOCITY = 0.00001f;
#endif

const float PLATFORMS_WIDTH = 0.4f;
const float PLATFORMS_HEIGHT = 0.15f;
const int PLATFORMS_COUNT = 20;

Platform platforms[PLATFORMS_COUNT];
Player player;

const int GRID_WIDTH = (int)(2.0f / 0.4f) + 5; // CH: changed this in order to evenly generate the platforms
const int GRID_HEIGHT = (int)(2.0f / 0.15f) - 1;

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
}


void HandleMouse()
{
	if(userMouse.IsLeftButtonDown())
	{
		player.Jump();
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

	
glm::vec2 platformsMinCorners[PLATFORMS_COUNT];
glm::vec2 platformsMaxCorners[PLATFORMS_COUNT];

Cake cakes[5];

void InitObjects()
{
	RandomizePlatforms(platforms, PLATFORMS_COUNT);

	glm::vec2 playerPosAtPlatform = glm::vec2(2.0f, 2.0f);

	for(int currPlatform = 0; currPlatform < PLATFORMS_COUNT; currPlatform++)
	{
		platforms[currPlatform].Init();
		platformsMinCorners[currPlatform] = platforms[currPlatform].GetMinCorner();
		platformsMaxCorners[currPlatform] = platforms[currPlatform].GetMaxCorner();

		// CH: Added this to ensure that the player's position is at the first platform
		if(playerPosAtPlatform.x > platforms[currPlatform].GetMaxCorner().x)
		{
			playerPosAtPlatform.x = platforms[currPlatform].GetMaxCorner().x;
			playerPosAtPlatform.y = platforms[currPlatform].GetMaxCorner().y;
		}
	}
	srand((unsigned) time(0));

	for(int i = 0; i < 5; i++)
	{
		int platformIndex = rand() % PLATFORMS_COUNT;

		float cakePosY = platforms[platformIndex].GetMaxCorner().y;
		float cakePosX = platforms[platformIndex].GetMaxCorner().x - platforms[platformIndex].GetWidth() / 2.0f;

		cakes[i] = Cake(glm::vec2(cakePosX, cakePosY), 0.05f, 0.0001f, 0.05f, 0.1f, 0.2f);
		cakes[i].Init();
	}

	glm::vec2 playerPos = playerPosAtPlatform;
	playerPos.x -= 0.1f;

	player = Player(playerPos, glm::vec2(0.01f, 0.0f),
					1.0,
					JUMP_UNITS,
					0.1f, 0.3f);
	player.Init();
}


DWORD nextGameTick;
int loops;

void Init()
{
	nextGameTick = GetTickCount();	


	InitializeShaders("VertexShader.vert", "FragmentShader.frag");

	InitObjects();

	// CH: the fat meter
	fatMeter = Sprite(glm::vec2(0.65f, -0.95f), glm::vec4(), 0.3f, 0.1f);
	fatMeter.Init();
	fatMeter.AddTexture("../data/fatbar.png");
	fatMeter.ChangeTexture("../data/fatbar.png");

	fatMeterArrow = FatMeterArrow(glm::vec2(0.65f, -0.95f), "../data/fatbar-arrow.png");

	

	jumpArrow = Sprite(glm::vec2(), glm::vec4(), 0.1f, 0.2f);
	jumpArrow.Init();
	jumpArrow.AddTexture("../data/jump-arrow.png");
	jumpArrow.ChangeTexture("../data/jump-arrow.png");


	jumpArrowRight = Sprite(glm::vec2(), glm::vec4(), 0.1f, 0.2f);
	jumpArrowRight.Init();
	jumpArrowRight.AddTexture("../data/jump-arrow-r.png");
	jumpArrowRight.ChangeTexture("../data/jump-arrow-r.png");


	jumpArrowDiag = Sprite(glm::vec2(), glm::vec4(), 0.1f, 0.2f);
	jumpArrowDiag.Init();
	jumpArrowDiag.AddTexture("../data/jump-arrow-diag.png");
	jumpArrowDiag.ChangeTexture("../data/jump-arrow-diag.png");


	clouds[0] = Cloud(1.1f, 0.0f, "../data/cloud2.png");
	clouds[1] = Cloud(0.6f, 0.0f, "../data/cloud2.png");
	clouds[2] = Cloud(-0.6f, 0.5f, "../data/cloud2.png");
	clouds[3] = Cloud(0.6f, 0.5f, "../data/cloud2.png");
	clouds[4] = Cloud(-0.3f, -0.5f, "../data/cloud2.png");
	clouds[5] = Cloud(0.4f, -0.5f, "../data/cloud2.png");

	glutMouseFunc(HandleMouseButtons);
	glutMotionFunc(HandleActiveMovement);
	glutPassiveMotionFunc(HandlePassiveMovement);
}

float _x = 0.0f;
float _y = 0.0f;

const float EPSILON = 0.0001f;

void UpdatePlatforms()
{
	for(int i = 0; i < PLATFORMS_COUNT; i++)
	{
		if(platforms[i].IsOutOfWindow())
		{
			for(int col = 0; col < GRID_WIDTH; col++)
			{
				for(int row = 0; row < GRID_HEIGHT; row++)
				{
					glm::vec2 positionVector = platformsGrid[col][row].position - platforms[i].GetMinCorner();
					float distance = glm::length(positionVector);

					if(distance <= EPSILON || distance >= -EPSILON)
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

			platformPos.x += 2.0f;
			platforms[i] = Platform(platformPos,
									PLATFORMS_WIDTH, PLATFORMS_HEIGHT,
									PLATFORMS_LEFT_VELOCITY);
			platforms[i].Init();
		}

		platforms[i].Update();
		platforms[i].Render(theProgram);
	}
}

void UpdateCakes()
{
	for(int i = 0; i < 5; i++)
	{
		if(cakes[i].IsOutOfWindow() || cakes[i].IsEaten())
		{
			int platformIndex = rand() % PLATFORMS_COUNT;

			float cakePosY = platforms[platformIndex].GetMaxCorner().y;
			float cakePosX = platforms[platformIndex].GetMaxCorner().x - platforms[platformIndex].GetWidth() / 2.0f;

			cakes[i] = Cake(glm::vec2(cakePosX, cakePosY), 0.05f, 0.0001f, 0.05f, 0.1f, 0.2f);
			cakes[i].Init();
		}
		
		if(player.GetCollisionBody().IsCollided(cakes[i].GetCollisionBody()))
		{
			player.AddFat(cakes[i].GetFat());
			cakes[i].SetIsEaten(true);
		}

		cakes[i].Update(PLATFORMS_LEFT_VELOCITY);
		cakes[i].Render(theProgram);
	}
}

float GetMappedToARange(float value,
						float leftMin, float leftMax,
						float rightMin, float rightMax)
{
	float leftSpan = leftMax - leftMin;
	float rightSpan = rightMax - rightMin;

	float valueScaled = (value - leftMin) / leftSpan;

	return rightMin + (valueScaled * rightSpan);
}

void Update()
{
	for(int i = 0; i < 6; i++)
	{
		clouds[i].Update(CLOUD_LEFT_VELOCITY);
	}

	UpdatePlatforms();
	UpdateCakes();

	player.Update(platforms, PLATFORMS_COUNT);

	
	if(player.IsDead())
	{
		glutLeaveMainLoop();
	}

	if(player.GetPosition().y >= 1.0f && player.GetPosition().x >= 1.0f)
	{
		jumpArrowDiag.UpdateData(glm::vec2(0.9f, 0.8f));
	}
	else if(player.GetPosition().y >= 1.0f)
	{
		if(player.GetPosition().x >= 0.9f)
		{
			jumpArrow.UpdateData(glm::vec2(0.9f, 0.8f));
		}
		else
		{
			jumpArrow.UpdateData(glm::vec2(player.GetPosition().x, 0.8f));
		}
	}
	else if(player.GetPosition().x >= 1.0f)
	{
		if(player.GetPosition().y >= 0.8f)
		{
			jumpArrowRight.UpdateData(glm::vec2(0.9f, 0.8f));
		}
		else
		{
			jumpArrowRight.UpdateData(glm::vec2(0.9f, player.GetPosition().y));
		}
	}

	float newX = player.GetFat();
	fatMeterArrow.Update(GetMappedToARange(newX, 0.0f, 2.0f, 0.35f, 2.95f));
}

void Render()
{
	for(int i = 0; i < 6; i++)
	{
		clouds[i].Update(CLOUD_LEFT_VELOCITY);
		clouds[i].Render(theProgram);
	}

	if(player.GetPosition().x >= 1.0f)
	{
		jumpArrowRight.Draw(theProgram);
	}
	else if(player.GetPosition().y >= 1.0f)
	{
		jumpArrow.Draw(theProgram);
	}
	else if(player.GetPosition().y >= 1.0f && player.GetPosition().x >= 1.0f)
	{
		jumpArrowDiag.Draw(theProgram);
	}

	UpdatePlatforms();
	UpdateCakes();	

	player.Update(platforms, PLATFORMS_COUNT);
	player.Render(theProgram);
		
	fatMeter.Draw(theProgram);
	fatMeterArrow.Render(theProgram);
}

void Display()
{
	//glClearColor(0.83f, 0.94f, 0.96f, 1.0f);
	glClearColor(0.76f, 0.91f, 0.94f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	loops = 0;
	while(GetTickCount() > nextGameTick && loops < MAX_FRAMESKIP)
	{
		Update();

		nextGameTick += SKIP_TICKS;
		loops++;
	}
	
	Render();


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