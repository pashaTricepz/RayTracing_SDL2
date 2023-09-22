
#include "constant.h"

RT::Normal::Constant::Constant()
{
	m_displacement = glm::dvec3(0.0);
}

RT::Normal::Constant::~Constant()
{
}

glm::dvec3 RT::Normal::Constant::ComputePerturbation(const glm::dvec3& normal, const glm::dvec2& uvCoords)
{
	return PerturbNormal(normal, m_displacement);
}
