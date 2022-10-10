#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <classes/filesystem.h>
#include <classes/shader.h>

#include <iostream>

class TriangleScene
{
public:
    TriangleScene(GLFWwindow* window);
    //~TriangleScene();
    void UpdateData(float deltatime);
    void Render();
private:
    GLFWwindow* m_window;
    // build and compile our shader program
    // ------------------------------------
    unsigned int VAO;
    unsigned int VBO, IBO;
    Shader shaderProgram;

    static float testVerticesNoInd[18];
};

