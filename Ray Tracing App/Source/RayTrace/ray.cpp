#include "ray.h"

RT::Ray::Ray()
{
	m_point1 = glm::dvec3(0.0);
	m_point2 = glm::dvec3(0.0, 0.0, 1.0);


	m_lab = m_point2 - m_point1;
}

RT::Ray::Ray(const glm::dvec3& point1, glm::dvec3& point2)
{
	m_point1 = point1;
	m_point2 = point2;

	m_lab = m_point2 - m_point1;
}

glm::dvec3 RT::Ray::GetPoint1() const
{
	return m_point1;
}

glm::dvec3 RT::Ray::GetPoint2() const
{
	return m_point2;
}
