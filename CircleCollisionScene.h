#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//#include <classes/filesystem.h>
#include <classes/shader.h>
#include <classes/camera.h>

#include <iostream>
#include <vector>

#include "Circle2D.h"
#include "Scene.h"

class CircleCollisionScene : public Scene
{
public:
    CircleCollisionScene();
    void UpdateData(float deltatime) override;
    void Render() override;
    void RenderImGUI() override;
private:
    void GenerateCircles(unsigned int circleAmountToGenerate, unsigned int lightCountToGenerate);
    bool CirclesIntersect(Circle2D circle1, Circle2D circle2);
    void DrawShadows(unsigned int lightIndex, float shadowStrength);
    void RenderCircle(Circle2D circle);
    void PerformPhysicsCycle(float deltatime);
    void CollideCircles(Circle2D &circle1, Circle2D &circle2);
    std::vector<Circle2D> circleArray;

    //Graphics attributes
    Shader circleShader, shadowShader, quadShader;

    unsigned int circleVAO, circleVBO, circleIBO, shadowVAO;
    unsigned int quadVBO, quadVAO;
    std::vector<float> quadVertices;
    std::vector<unsigned int> quadIndices;
    //holds the position and radius instance attribs for shadow rendering
    std::vector<float> circleInstanceData;
    float timeBank;
    float shadowStrength;
};

