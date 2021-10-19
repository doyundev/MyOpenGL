#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class RenderingSystem
{
public:
	RenderingSystem();
	~RenderingSystem();

	GLFWwindow* GetWindow();
	void BeginRender();
	void EndRender();
	void framebuffer_size_callback(GLFWwindow *window, int width, int height);

private:
	const int WIN_WIDTH = 800;
	const int WIN_HEIGHT = 600;
	GLFWwindow* window = nullptr;
};