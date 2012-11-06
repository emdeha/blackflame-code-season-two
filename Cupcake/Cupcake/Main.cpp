#include "Main.h"
#include "../Platform/Platform.h"
#include "../Player/Player.h"

#include "../Mouse/Mouse.h"


Platform platforms[10];
Player player;

Mouse userMouse;


GLuint theProgram;
GLuint offsetUniform;
GLuint isPlayerUniform;


float jumpUnits = 0.0005f;

const float PLATFORMS_WIDTH = 0.4f;
const float PLATFORMS_HEIGHT = 0.15f;
const int PLATFORMS_COUNT = 10;


bool IsCollided(int indexOfPlatformToCollide, Platform allPlatforms[], int platformCount)
{
	glm::vec2 platformMinCorner = platforms[indexOfPlatformToCollide].GetMinCorner();
	glm::vec2 platformMaxCorner = platforms[indexOfPlatformToCollide].GetMaxCorner();

	for(int currPlatform = 0; currPlatform < platformCount; currPlatform++)
	{		
		if(currPlatform == indexOfPlatformToCollide)
		{
			break;
		}

		glm::vec2 currentPlatformMinCorner = platforms[platformCount].GetMinCorner();
		glm::vec2 currentPlatformMaxCorner = platforms[platformCount].GetMaxCorner();

		if(platformMinCorner.x >= currentPlatformMinCorner.x &&
		   platformMinCorner.y >= currentPlatformMinCorner.y &&
		   platformMinCorner.x <= currentPlatformMaxCorner.x &&
		   platformMinCorner.y <= currentPlatformMaxCorner.y ||
		   platformMaxCorner.x >= currentPlatformMinCorner.x &&
		   platformMaxCorner.y >= currentPlatformMinCorner.y &&
		   platformMaxCorner.x <= currentPlatformMaxCorner.x &&
		   platformMaxCorner.y <= currentPlatformMaxCorner.y)
		{
			return true;
		}		
	}

	return false;
}

void RandomizePlatforms(Platform platformArray[], int platformCount)
{
	for(int currPlatform = 0; currPlatform < platformCount; currPlatform++)
	{
		glm::vec2 newPlatformPosition;
		newPlatformPosition.x = 2 * ((float) rand() / RAND_MAX) - 1;
		newPlatformPosition.y = 2 * ((float) rand() / RAND_MAX) - 1;
		
		platformArray[currPlatform] =
			Platform(newPlatformPosition,
					 PLATFORMS_WIDTH, PLATFORMS_HEIGHT);

		while(IsCollided(currPlatform, platformArray, platformCount))
		{
			newPlatformPosition.x = 2 * ((float) rand() / RAND_MAX) - 1;
			newPlatformPosition.y = 2 * ((float) rand() / RAND_MAX) - 1;

			platformArray[currPlatform] =
				Platform(newPlatformPosition,
						 PLATFORMS_WIDTH, PLATFORMS_HEIGHT);
		}
	}
}


void HandleMouse()
{
	if(userMouse.IsLeftButtonDown())
	{
		player.Jump(jumpUnits);
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

void InitObjects()
{
	RandomizePlatforms(platforms, PLATFORMS_COUNT);

	for(int currPlatform = 0; currPlatform < PLATFORMS_COUNT; currPlatform++)
	{
		platforms[currPlatform].Init();
		platformsMinCorners[currPlatform] = platforms[currPlatform].GetMinCorner();
		platformsMaxCorners[currPlatform] = platforms[currPlatform].GetMaxCorner();
	}

	player = Player(glm::vec2(0.0f, 0.2f), glm::vec2(0.01f, 0.0f),
					0.1f, 0.15f);

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


	for(int currPlatform = 0; currPlatform < PLATFORMS_COUNT; currPlatform++)
	{
		platforms[currPlatform].Render(theProgram);
	}

	player.Update(platformsMinCorners, platformsMaxCorners, PLATFORMS_COUNT);

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

	case 32:
		player.Jump(jumpUnits);
		break;
	}
}

unsigned int Defaults(unsigned int displayMode, int &width, int &height)
{
	return displayMode;
}