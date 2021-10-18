#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

const int WINSIZE_X = 800;
const int WINSIZE_Y = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINSIZE_X, WINSIZE_Y, "Learn OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		system("pause");
		return -1;
	}

	framebuffer_size_callback(window, WINSIZE_X, WINSIZE_Y);

	// �ﰢ���� �׷�����
	float vertices[] = {
		-0.25f - 0.5f, -0.25f, 0.0f,	// pos
		1.0f,0.0f, 0.0f,			// color
		0.0f - 0.5f, 0.25f, 0.0f,	// pos
		0.0f,1.0f, 0.0f,			// color
		0.25f - 0.5f,  -0.25f, 0.0f,	// pos
		0.0f,0.0f, 1.0f				// color
	};

	float vertices2[] = {
		-0.25f + 0.25f, -0.25f, 0.0f,
		1.0f,1.0f, 0.0f,		// color
		0.0f + 0.25f, 0.25f, 0.0f,
		1.0f,1.0f, 0.0f,		// color
		0.25f + 0.25f,  -0.25f, 0.0f,
		1.0f,1.0f, 0.0f			// color
	};

	unsigned int indices[] = {
		0,1,2,
		0,2,3
	};

	// Triangle #1
	// Vertex Buffer Object
	unsigned int VBO[2] = {};
	glGenBuffers(2, VBO); // ID�� �����ϴ� ����

	// Vertex Array Object
	unsigned int VAO[2] = {};
	glGenVertexArrays(2, VAO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// EBO
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	// ���̴� Ŭ���� �غ�
	Shader* myShader = new Shader("./Shader/vs.glsl", "./Shader/fs.glsl");

	// ���ӷ���?
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		{	
			// �ε����� �׷��ִ� ���
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// VAO �� �׷��ִ� ���
			myShader->use();
			int uniformLocation = glGetUniformLocation(myShader->ID, "tintColor");
			glUniform3f(uniformLocation, 1.0f, 0.0f, 0.0f);
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glUniform3f(uniformLocation, 1.0f, 1.0f, 0.0f);
			glBindVertexArray(VAO[1]);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			// �ּ��� Ǯ�� ���̾����������� ���δ�.
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glfwSwapBuffers(window); // ���� ���۸�
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	glDeleteProgram(myShader->ID);
	delete myShader; myShader = 0;

	glfwTerminate();
	return 0;
}

// ������ ȭ�� ����� �������� �� ȣ��Ǵ� �ݹ�
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	// ESC�� ������ �� �����찡 ���������Ѵ�
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}