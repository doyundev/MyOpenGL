#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "./imgui.h"
#include "./imgui_impl_glfw.h"
#include "./imgui_impl_opengl3.h"

// GL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	// 셰이더
	Shader* myShader = new Shader("./Shader/color/vs.glsl", "./Shader/color/fs.glsl");
	Shader* texShader = new Shader("./Shader/texture/vs.glsl", "./Shader/texture/fs.glsl");

	// 버텍스 정보
	float vertices[] = {
		-0.25f - 0.5f, -0.25f, 0.0f,	// pos
		1.0f,0.0f, 0.0f,			// color
		0.0f - 0.5f, 0.25f, 0.0f,	// pos
		0.0f,1.0f, 0.0f,			// color
		0.25f - 0.5f,  -0.25f, 0.0f,	// pos
		0.0f,0.0f, 1.0f				// color
	};

	// 텍스쳐 사용하는 놈으로 그린다
	// 0 1
	// 2 3
	float vertices2[] = {
		-0.5f, 0.5f, 0.0f,	1.0f,1.0f,0.0f,	0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,	1.0f,0.0f,0.0f,	1.0f,0.0f,
		-0.5f, -0.5f, 0.0f,	0.0f,0.0f,1.0f,	0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	0.0f,1.0f,0.0f,	1.0f,1.0f
	};

	unsigned int indices[] = {
		0,1,2,
		1,3,2
	};

	// 1) VAO 및 버퍼 세팅
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 2) 어트리뷰트 설정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float))); // UV
	glEnableVertexAttribArray(2);

	// 3) 텍스쳐 생성
	unsigned int texture1,texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	glBindTexture(GL_TEXTURE_2D, texture1);
	unsigned char *data = stbi_load("./Resources/wall.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // 텍스쳐 로드
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); // 로드를 완료했으므로 해제해준다.


	// 5) 텍스쳐2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glGenTextures(1, &texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texture2);
	data= stbi_load("./Resources/awesomeface.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // 텍스쳐 로드
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data); // 로드를 완료했으므로 해제해준다.

	texShader->use();
	texShader->setInt("texture1", 0);
	texShader->setInt("texture2", 1);

	float mixRate = 0.2f;
	float rotZ = 0.0f;

	// 게임루프?
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Setting");
		{
			ImGui::DragFloat("Mix Rate", &mixRate, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Rot Z", &rotZ, 0.1f, 0.0f, 360.0f);
		}
		ImGui::End();

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		{	
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);

			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::rotate(trans, glm::radians(rotZ), glm::vec3(0, 0, 1));
			trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));

			texShader->use();
			texShader->setFloat("mixRate",mixRate);
			unsigned int transformLoc = glGetUniformLocation(texShader->ID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// 주석을 풀면 와이어프레임으로 보인다.
			// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		// Render UI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(window); // 더블 버퍼링
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteProgram(myShader->ID);
	delete myShader; myShader = 0;

	glDeleteProgram(texShader->ID);
	delete texShader; texShader = 0;

	glfwTerminate();
	return 0;
}

// 유저가 화면 사이즈를 변경했을 때 호출되는 콜백
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	// ESC를 눌렀을 때 윈도우가 닫히도록한다
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}