#include "shaderManager.h"

ShaderManager::ShaderManager()
{
	ID = 0;
}

void ShaderManager::UseProgram()
{
	glUseProgram(ID);
}

void ShaderManager::LoadAndCompileShaders(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	try {
		std::ifstream vertexFile(vertexPath);
		std::ifstream fragmentFile(fragmentPath);

		std::stringstream vertexStream, fragmentStream;

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();
	}
	catch (std::ifstream::failure e) {
		printf("ERROR READING SHADER FILE");
	}

	const char* realVertexShader = vertexCode.c_str();
	const char* realFragmentShader = fragmentCode.c_str();

	unsigned int vertexShader, fragmentShader;
	int success;
	char infolog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &realVertexShader, NULL);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &realFragmentShader, NULL);

	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		printf("Vertex Shader compilation failed. Error: %s\n", infolog);
	}

	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		printf("Fragment Shader compilation failed. Error: %s\n", infolog);
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infolog);
		printf("Failed to link shader program. Error: %s\n", &infolog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	printf("Shaders compiled successfully.\n");
}

GLuint ShaderManager::GetProgram()
{
	return ID;
}

void ShaderManager::SetFloat4(std::string name, float x, float y, float z, float alpha)
{
	glUniform4f(glGetUniformLocation(GetProgram(), name.c_str()), x, y, z, alpha);
}

void ShaderManager::SetFloat3(std::string name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(GetProgram(), name.c_str()), x, y, z);
}