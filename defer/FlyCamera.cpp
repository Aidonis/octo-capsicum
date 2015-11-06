#include "Camera.h"

#include "../nsfwgl/Window.h"

#define GLM_SWIZZLE
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glfw\glfw3.h>

#include <iostream>

void FlyCamera::update()
{
	std::cout << glm::to_string(transform[3]) << std::endl;

	speed = 5;
	GLFWwindow *window = glfwGetCurrentContext();

	float deltaTime = nsfw::Window::instance().getDeltaTime();

	float frameSpeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? deltaTime * speed * 2 : deltaTime * speed;

	// translate
	if (glfwGetKey(window, 'W') == GLFW_PRESS)
		transform[3] -= transform[2] * frameSpeed;
	if (glfwGetKey(window, 'S') == GLFW_PRESS)
		transform[3] += transform[2] * frameSpeed;
	if (glfwGetKey(window, 'D') == GLFW_PRESS)
		transform[3] += transform[0] * frameSpeed;
	if (glfwGetKey(window, 'A') == GLFW_PRESS)
		transform[3] -= transform[0] * frameSpeed;
	if (glfwGetKey(window, 'Q') == GLFW_PRESS)
		transform[3] += transform[1] * frameSpeed;
	if (glfwGetKey(window, 'E') == GLFW_PRESS)
		transform[3] -= transform[1] * frameSpeed;

	// rotation
	static bool sbMouseButtonDown = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		// HACK: statics to store the prev mouse positions
		static double prevMouseX = 0;
		static double prevMouseY = 0;

		if (sbMouseButtonDown == false)
		{
			sbMouseButtonDown = true;
			glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
		}

		// acquire current frame
		double mouseX = 0, mouseY = 0;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		double deltaX = mouseX - prevMouseX;
		double deltaY = mouseY - prevMouseY;

		// store current frame data for the next frame
		prevMouseX = mouseX;
		prevMouseY = mouseY;

		glm::mat4 mat;

		//std::cout << "deltaX:" << deltaX << "\n";
		//std::cout << "deltaY:" << deltaY << "\n \n";

		// pitch
		if (deltaY != 0)
		{
			mat = glm::rotate((float)-deltaY * deltaTime * 10, glm::vec3(transform[0].x, transform[0].y, transform[0].z));
			//mat = glm::axisAngleMatrix(vec3(transform[0].xyz), (float)-deltaY * deltaTime);
			transform[0] = mat * transform[0];
			transform[1] = mat * transform[1];
			transform[2] = mat * transform[2];
			//transform *= mat;
		}

		// yaw
		if (deltaX != 0)
		{
			mat = glm::rotate((float)-deltaX * deltaTime * 10, glm::vec3(0, 1, 0));
			//mat = glm::axisAngleMatrix(glm::vec3(0, 1, 0), (float)-deltaX * deltaTime);
			transform[0] = mat * transform[0];
			transform[1] = mat * transform[1];
			transform[2] = mat * transform[2];
			//transform *= mat;
		}
	}
	else
	{
		sbMouseButtonDown = false;
	}


}