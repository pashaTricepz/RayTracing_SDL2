#include "lightbase.h"

RT::LightBase::LightBase()
{
	m_color = glm::dvec3(1.0);
	m_location = glm::dvec3(0.0);
	m_intensity = 0.0;
}

RT::LightBase::~LightBase()
{
}

bool RT::LightBase::ComputeIllumination(const glm::dvec3& intPoint, const glm::dvec3& localNormal,
										const std::vector<std::shared_ptr<ObjectBase>>& objectList,
										const std::shared_ptr<ObjectBase>& currentObject,
										glm::dvec3& color, double& intensity)
{
	return false;
}
