#include "PhongTestScene.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "SphereCollisionScene.h"

extern Camera camera;
extern GLFWwindow* window;
PhongTestScene::PhongTestScene() :
//shaderProgram("shaders/PhongSceneShader.vert", "shaders/PhongSceneShader.frag", "shaders/PhongSceneShader.geom")
shaderProgram("shaders/SphereCollisionScene.vert", "shaders/SphereCollisionScene.frag")
/*
vertices {
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f
},
indices{
	0, 3, 1,
	3, 2, 1,
	3, 7, 6,
	3, 6, 2,
	5, 6, 7,
	5, 7, 4,
	0, 5, 4,
	0, 1, 5,
	1, 2, 6,
	1, 6, 5,
	0, 7, 3,
	0, 4, 7
}
*/
{
	SphereCollisionScene::GenerateSphereMesh(50, 50, vertices, indices);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void PhongTestScene::UpdateData(float deltatime)
{

}

void PhongTestScene::Render()
{
	glClearColor(0.3f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.use();

	// lighting
	glm::vec3 lightPos(1.4f, 2.0f, 3.0f);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 MVP = projection * view * model;
	shaderProgram.setMat4("MVP", MVP);
	shaderProgram.setMat4("model", model);
	shaderProgram.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	shaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shaderProgram.setVec3("lightPos", lightPos);
	shaderProgram.setVec3("viewPos", camera.Position);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(window);
	glfwPollEvents();
}


