#include "Circle2D.h"

Circle2D::Circle2D(float radius, glm::vec3 pos, glm::vec3 color, glm::vec3 velocity, float isLight) :
	m_pos(pos),
	m_radius(radius),
	m_mass(radius* radius* radius),
	m_color(color),
	m_velocity(velocity),
	m_excitement(0.),
	m_isLight(isLight)
{
}
