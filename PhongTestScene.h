#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <classes/filesystem.h>
#include <classes/shader.h>
#include <classes/camera.h>

#include <iostream>
#include <vector>

#include "Scene.h"

class PhongTestScene: public Scene
{
public:
    PhongTestScene();
    void UpdateData(float deltatime) override;
    void Render() override;
private:
    // build and compile our shader program
    // ------------------------------------
    unsigned int VAO;
    unsigned int VBO, IBO;
    unsigned int texture;

    Shader shaderProgram;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

