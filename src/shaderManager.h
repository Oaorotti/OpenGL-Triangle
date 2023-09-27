#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ShaderManager {
private:
	GLuint ID;
public:
	ShaderManager();
	void UseProgram();
	GLuint GetProgram();
	void LoadAndCompileShaders(const char* vertexPath, const char* fragmentPath);

	void SetFloat4(std::string name, float x, float y, float z, float alpha);
	void SetFloat3(std::string name, float x, float y, float z);
};