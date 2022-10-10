#include "TriangleScene.h"

float TriangleScene::testVerticesNoInd[18] = {
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f
};

TriangleScene::TriangleScene(GLFWwindow* window): m_window(window), shaderProgram("TriangleScene_shader.vert", "TriangleScene_shader.frag")
{
	float vertices[] = {
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f
	};
	// generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind vertex array
	glBindVertexArray(VAO);

	//bind and give data to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testVerticesNoInd), testVerticesNoInd, GL_STATIC_DRAW);

	//configure vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void TriangleScene::UpdateData(float deltatime)
{
}

void TriangleScene::Render()
{
	//clear screen
	glClearColor(0.1f, 0.1f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderProgram.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}
