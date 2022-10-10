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
class Circle2D
{
public:
	Circle2D(float radius, glm::vec3 pos, glm::vec3 color, glm::vec3 velocity, float isLight);

	//circle attributes
	float m_radius;
	float m_mass;
	float m_excitement;
	float m_isLight;
	glm::vec3 m_pos;
	glm::vec3 m_color;
	glm::vec3 m_velocity;
private:
};

