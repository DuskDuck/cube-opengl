
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<ft2build.h>
#include FT_FREETYPE_H

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

const unsigned int width = 800;
const unsigned int height = 800;

float xRotation = 0.0f;
float yRotation = 0.0f;

float faceColors[6][3];

const int NUM_KEYS = 6;

bool prevKeyState[NUM_KEYS] = { false };

GLfloat vertices[] =
{
	-1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  // Back 
	 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,

	-1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  // Front 
	 1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,

	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f,  // Left 
	-1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,

	 1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  // Right
	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
	 1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

	-1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  // Bottom
	 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,

	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  // Top
	 1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
};

GLuint indices[] =
{
	 0,  1,  2,  2,  3,  0, // Back
	 4,  5,  6,  6,  7,  4, // Front
	 8,  9, 10, 10, 11,  8, // Left
	12, 13, 14, 14, 15, 12, // Right
	16, 17, 18, 18, 19, 16, // Bottom
	20, 21, 22, 22, 23, 20  // Top
};

//Gen random color
void genColor(int face) {
	faceColors[face][0] = static_cast<float>(rand()) / RAND_MAX;
	faceColors[face][1] = static_cast<float>(rand()) / RAND_MAX;
	faceColors[face][2] = static_cast<float>(rand()) / RAND_MAX;

	for (int i = 0; i < 4; i++) {
		int vertexIdx = (face * 4 + i) * 6 + 3;
		vertices[vertexIdx + 0] = faceColors[face][0];
		vertices[vertexIdx + 1] = faceColors[face][1];
		vertices[vertexIdx + 2] = faceColors[face][2];
	}
}

void handlingInput(GLFWwindow* window, VBO VBO) {
	//Handle change color
	bool colorChanged = false;
	//NUMKEY
	for (int i = 0; i < NUM_KEYS; ++i) {
		int key = GLFW_KEY_1 + i;
		bool currentKeyState = glfwGetKey(window, key) == GLFW_PRESS;

		if (currentKeyState && !prevKeyState[i]) {
			genColor(i);
			colorChanged = true;
		}

		prevKeyState[i] = currentKeyState;
	}
	//NUMPAD
	for (int i = 0; i < 6; ++i) {
		int numpadKey = GLFW_KEY_KP_1 + i;
		bool currentNumpadKeyState = glfwGetKey(window, numpadKey) == GLFW_PRESS;

		if (currentNumpadKeyState && !prevKeyState[6 + i]) {
			genColor(i);
			colorChanged = true;
		}

		prevKeyState[6 + i] = currentNumpadKeyState;
	}

	//Handle rotation
	const float rotationSpeed = 0.02f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		xRotation -= rotationSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		xRotation += rotationSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		yRotation -= rotationSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		yRotation += rotationSpeed;
	}
	//update the VBO
	if (colorChanged) {
		VBO.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	}
}

int main()
{
	// Init GLFW
	glfwInit();

	//OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//CORE profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(width, height, "CubeSpin", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//window to the current context
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);


	//Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	//Generates Vertex Array Object, binds
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent modify
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//get scale attribute
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	// game loop
	while (!glfwWindowShouldClose(window))
	{
		//bg color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clean the back buffer and assign the new color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Activate Shader for openGL
		shaderProgram.Activate();
		//Input handling func
		handlingInput(window, VBO1);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, xRotation, glm::vec3(1.0f, 0.0f, 0.0f));  // Rotation x-axis
		model = glm::rotate(model, yRotation, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotation y-axis

		view = glm::translate(view, glm::vec3(0.0f, -0.0f, -8.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		//glUniform1f(uniID, 0.0f); //Set scale

		// Bind the VAO
		VAO1.Bind();
		// Draw 
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap back buffer with front buffer
		glfwSwapBuffers(window);

		glfwPollEvents();
	}


	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
