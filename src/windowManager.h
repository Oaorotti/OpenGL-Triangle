#pragma once
#include <iostream>
#include <glad/glad.h>

#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "shaderManager.h"

class WindowManager {
private:
	GLFWwindow* window = nullptr;
	GLuint VAO, VBO, EBO, texture;
	ShaderManager shader;
	glm::vec3 newPos;

	int windowWidth;
	int windowHeight;

	void createWindow(int width, int height, std::string title);
	void makeContextCurrent();
public:
	WindowManager(int width, int height, std::string title);
	~WindowManager();

	void Tick();
	void Update();
	void Start();
	void SetWindowSize(int width, int height);
	GLFWwindow* GetWindow();
	int GetWidth();
	int GetHeight();
};