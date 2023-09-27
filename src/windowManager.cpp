#include "windowManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
	WindowManager* manager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));

	if (manager != nullptr) {
		manager->SetWindowSize(width, height);
	}
}

WindowManager::WindowManager(int width, int height, std::string title) {
	createWindow(width, height, title);
}

WindowManager::~WindowManager() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void WindowManager::Tick() {
	Start();
	while (!glfwWindowShouldClose(this->window)) {
		Update();
	}
}

void WindowManager::Start() {
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {  
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};

	VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint texture0 = glGetUniformLocation(shader.GetProgram(), "tex0");
	shader.UseProgram();
	glUniform1i(texture0, 0);

	EBO = 0;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("images/texture.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("failed to load texture.\n");
	}

	stbi_image_free(data);

	shader.LoadAndCompileShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
}

void WindowManager::Update() {
	glClearColor(0.18f, 0.36f, 0.36f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.UseProgram();
	glBindVertexArray(VAO);

	float timeValue = glfwGetTime();
	float xOffset = (sin(timeValue)) * 0.5f;

	newPos = glm::vec3(xOffset, 0.0f, 1.0f);

	shader.SetFloat3("myPos", newPos.x, newPos.y, newPos.z);

	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void WindowManager::SetWindowSize(int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	glViewport(0, 0, windowWidth, windowHeight);
}

void WindowManager::createWindow(int width, int height, std::string title) {
	glfwSetErrorCallback(error_callback);

	windowWidth = width;
	windowHeight = height;

	if (!glfwInit()) {
		printf("couldn't initialize glfw.\n");
		glfwTerminate();
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), 0, 0);

	if (window == nullptr) {
		printf("failed to create window.\n");
		glfwTerminate();
		return;
	}

	makeContextCurrent();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("failed to initialize GLAD\n");
		return;
	}

	glViewport(0, 0, 1280, 720); // hardcoded

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, window_resize_callback);

	glfwSwapInterval(1);

	printf("GL Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("GL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void WindowManager::makeContextCurrent()
{
	glfwMakeContextCurrent(window);
}

GLFWwindow* WindowManager::GetWindow() {
	return window;
}

int WindowManager::GetWidth() {
	return windowWidth;
}

int WindowManager::GetHeight() {
	return windowHeight;
}