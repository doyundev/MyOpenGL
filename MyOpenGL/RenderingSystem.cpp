#include "RenderingSystem.h"
#include <iostream>


RenderingSystem::RenderingSystem()
{
	glfwInit();

	// 버전 3.3 의 core 를 사용한다는 정보를 전달한다.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello World", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to Create GLFW Window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to Initialize GLAD" << std::endl;
		return;
	}

	this->framebuffer_size_callback(window, WIN_WIDTH, WIN_HEIGHT);
}

RenderingSystem::~RenderingSystem()
{
	glfwTerminate();
}

GLFWwindow * RenderingSystem::GetWindow()
{
	return this->window;
}

void RenderingSystem::BeginRender()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingSystem::EndRender()
{
	glfwPollEvents();
	glfwSwapBuffers(this->window);
}

void RenderingSystem::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}
