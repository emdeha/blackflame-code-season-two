#include "Main.h"
#include "../Platform/Platform.h"
#include "../Player/Player.h"


Platform platforms[3];
Player player;


GLuint theProgram;
GLuint offsetUniform;
GLuint isPlayerUniform;



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


void Init()
{
	InitializeShaders("VertexShader.vert", "FragmentShader.frag");

	platforms[0] = Platform(glm::vec2(0.0f, 0.0f), 
							0.4f, 0.15f);
	platforms[1] = Platform(glm::vec2(-0.7f, -0.5f),	
							0.4f, 0.15f);
	platforms[2] = Platform(glm::vec2(0.5f, 0.5f),
							0.4f, 0.15f);
	platforms[0].Init();
	platforms[1].Init();
	platforms[2].Init();


	player = Player(glm::vec2(0.0f, 0.2f), glm::vec2(0.1f, 0.0001f),
					0.1f, 0.15f);

	player.Init();
	InitializeVertexBuffer();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

float _x = 0.0f;
float _y = 0.0f;

void Display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	

	platforms[0].Render(theProgram);
	platforms[1].Render(theProgram);
	platforms[2].Render(theProgram);



	glm::vec2 minCorners[] = 
	{
		platforms[0].GetMinCorner(),
		platforms[1].GetMinCorner(),
		platforms[2].GetMinCorner(),
	};
	glm::vec2 maxCorners[] =
	{
		platforms[0].GetMaxCorner(),
		platforms[1].GetMaxCorner(),
		platforms[2].GetMaxCorner(),
	};


	player.Update(minCorners, maxCorners, 3);

	player.Render(theProgram);

	/*glUseProgram(theProgram);

	glUniform2f(offsetUniform, _x, _y);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUseProgram(0);*/

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
		//_x += 0.1f;
		player.MoveLeft(0.05f);
		break;
	case 'd':
		//_x -= 0.1f;
		player.MoveRight(0.05f);
		break;

	case 32:
		player.Jump(1000);
		break;
	}
}

unsigned int Defaults(unsigned int displayMode, int &width, int &height)
{
	return displayMode;
}