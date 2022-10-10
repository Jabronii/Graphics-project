#include "Sphere.h"

Sphere::Sphere(float radius, glm::vec3 pos, glm::vec3 color, glm::vec3 velocity, bool isLight) :
	m_pos(pos),
	m_radius(radius),
	m_mass(radius* radius* radius),
	m_color(color),
	m_velocity(velocity),
	m_excitement(0.),
	m_isLight(isLight)
{
}
