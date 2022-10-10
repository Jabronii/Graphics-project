#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <classes/filesystem.h>
#include <classes/shader.h>
#include <classes/camera.h>

#include <iostream>
#include <vector>

#include "Sphere.h"
#include "Scene.h"

class SphereCollisionScene : public Scene
{
public:
    SphereCollisionScene();
    void UpdateData(float deltatime) override;
    void Render() override;
    static void GenerateSphereMesh(unsigned int horizontalRows, unsigned int verticalRows, std::vector<float>& vertices, std::vector<unsigned int>& indices);
private:
    void GenerateSpheres(unsigned int sphereAmountToGenerate, unsigned int lightCount);

    //physics
    bool SpheresIntersect(Sphere sphere1, Sphere sphere2);
    void PerformPhysicsCycle(float deltatime);
    void CollideSpheres(Sphere& sphere1, Sphere& sphere2);

    //graphics
    void RenderSphere(Sphere sphere);
    void RenderLightSphere(Sphere sphere);
    void RenderEnclosure();

    std::vector<Sphere> sphereArray;

    //Graphics attributes

    Shader spheresShader;
    Shader lightShader;
    unsigned int sphereVAO, sphereVBO, sphereIBO;
    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;

    Shader hdrShader;
    unsigned int quadVAO, quadVBO;
    unsigned int hdrFBO;
    unsigned int colorBuffer;
    std::vector<float> quadVertices;

    unsigned int enclosureVAO, enclosureVBO, enclosureIBO;
    std::vector<float> enclosureVertices;
    std::vector<unsigned int> enclosureIndices;
    float timeBank;
};

