#include "simplerough.h"

RT::Normal::SimpleRough::SimpleRough()
{
	std::random_device randDev;
	std::seed_seq seed{randDev(), randDev(), randDev(), randDev()};
	m_p_randGen = std::make_shared<std::mt19937>(seed);
}

RT::Normal::SimpleRough::~SimpleRough()
{
}

glm::dvec3 RT::Normal::SimpleRough::ComputePerturbation(const glm::dvec3& normal, const glm::dvec2& uvCoords)
{
	std::uniform_real_distribution<double> randomDist(-m_amplitude, m_amplitude);

	double x = randomDist(*m_p_randGen);
	double y = randomDist(*m_p_randGen);
	double z = randomDist(*m_p_randGen);

	glm::dvec3 perturbation(x, y, z);

	return PerturbNormal(normal, perturbation);
}
