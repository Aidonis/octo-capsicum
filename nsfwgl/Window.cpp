#include "nsfw.h"

void APIENTRY oglErrorDefaultCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	// if 'GL_DEBUG_OUTPUT_SYNCHRONOUS' is enabled, you can place a
	// breakpoint here and the callstack should reflect the problem location!

	std::cerr << message << std::endl;
}

void nsfw::Window::init(unsigned width, unsigned height)
{
	if (glfwInit() == false) {
		return;
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	window = glfwCreateWindow(width, height, "nsfwGL", nullptr, nullptr);

	this->width = width;
	this->height = height;

	if (window == nullptr) {
		glfwTerminate();
	}
	
	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return;
	}

#ifdef _DEBUG
	if (glDebugMessageCallback)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(oglErrorDefaultCallback, nullptr);

		GLuint unusedIDs = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIDs, true);
	}
	else
	{
		std::cerr << "Failed to subscribe to glDebugMessageCallback." << std::endl;
	}
#endif

	//TODO_D("Should create and set an active windowing context. ONLY GLFW! No GL!");
}

void nsfw::Window::step()
{
	glfwSwapBuffers(window);
	glfwPollEvents();

	//TODO_D("GLFW poll events and swap buffers is all that should really be here! No GL!");
}

void nsfw::Window::term()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

float nsfw::Window::getTime() const
{
	//Look into tick
	//TODO();
	return glfwGetTime();
}

bool nsfw::Window::getKey(unsigned k) const
{
	//TODO();
	if(glfwGetKey(window, k) == GLFW_PRESS){
		return true;
	}
	else{
		return false;
	}
	
}

bool nsfw::Window::getShouldClose() const
{
	if (glfwWindowShouldClose(window) || getKey(256)) {
		return true;
		
	}

	/*int i = 1;
	if (i == 1){
		return true;
	}*/
	//TODO();
	return false;
	
}

unsigned nsfw::Window::getWidth() const
{
	//TODO();
	return width;
}

unsigned nsfw::Window::getHeight() const
{
	//TODO();
	return height;
}

glm::mat4 nsfw::Window::getTexelAdjustmentMatrix() const
{
	TODO_D("Implemented, not tested.");

	glm::vec3 texelSize = 1.0f/glm::vec3(width,height,0);
	glm::vec3 halfTexel = texelSize * 0.5f; // bottom left

	// scale up to the appropriate size, then shift it to line up w/center of texels
	return glm::translate(halfTexel) * glm::scale(glm::vec3(1, 1, 1) - texelSize);	
}