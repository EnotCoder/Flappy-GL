#include "control.h"
#include <GLFW/glfw3.h>
#include <iostream>

bool IsKeyJustPressed(GLFWwindow* window, int glfwKey, int &prevKeyState)
{	
	int NowKeyState = glfwGetKey(window, glfwKey);
	if (NowKeyState == GLFW_PRESS && prevKeyState == GLFW_RELEASE)
	{	
		prevKeyState = NowKeyState;
		return true;
	}

	prevKeyState = NowKeyState;

	return false;
}