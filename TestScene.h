#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <classes/filesystem.h>
#include <classes/shader.h>

#include <iostream>

class TestScene
{
public:
    TestScene(GLFWwindow* window);
    ~TestScene();
    void UpdateData(float deltatime);
    void Render();
private:
    GLFWwindow* m_window;
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader;
    Shader lightCubeShader;
    // lighting
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    unsigned int VBO, cubeVAO;
    unsigned int lightCubeVAO;
};

