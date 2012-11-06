#include "stdafx.h"
#include "Mouse.h"


Mouse::Mouse()
{
	currentPosition = glm::ivec2();
	lastPosition = glm::ivec2();

	isLeftButtonPressed = false;
	isRightButtonPressed = false;

	isMouseWheelUp = false;
	isMouseWheelDown = false;
}

void Mouse::SetCurrentPosition(glm::ivec2 newCurrentPosition)
{
	currentPosition = newCurrentPosition;
}

glm::ivec2 Mouse::GetCurrentPosition()
{
	return currentPosition;
}
glm::ivec2 Mouse::GetLastPosition()
{
	return lastPosition;
}

void Mouse::OverrideLastPosition(glm::ivec2 newLastPosition)
{
	lastPosition = newLastPosition;
}


bool Mouse::IsLeftButtonDown()
{
	return isLeftButtonPressed;
}
bool Mouse::IsRightButtonDown()
{
	return isRightButtonPressed;
}

bool Mouse::IsMouseWheelUp()
{
	return isMouseWheelUp;
}
bool Mouse::IsMouseWheelDown()
{
	return isMouseWheelDown;
}

void Mouse::PressLeftButton()
{
	isLeftButtonPressed = true;
}
void Mouse::ReleaseLeftButton()
{
	isLeftButtonPressed = false;
}
void Mouse::PressRightButton()
{
	isRightButtonPressed = true;
}
void Mouse::ReleaseRightButton()
{
	isRightButtonPressed = false;
}

void Mouse::MouseWheelUp()
{
	isMouseWheelUp = true;
}
void Mouse::MouseWheelNotUp()
{
	isMouseWheelUp = false;
}
void Mouse::MouseWheelDown()
{
	isMouseWheelDown = true;
}
void Mouse::MouseWheelNotDown()
{
	isMouseWheelDown = false;
}