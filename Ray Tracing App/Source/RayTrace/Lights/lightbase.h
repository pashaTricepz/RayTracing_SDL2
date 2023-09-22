#pragma once

#include <vector>

#include "../ray.h"

#include "../Primitives/objectbase.h"

namespace RT {
	class LightBase {
	public:
		LightBase();

		virtual ~LightBase();

		virtual bool ComputeIllumination(const glm::dvec3& intPoint, const glm::dvec3& localNormal,
										 const std::vector<std::shared_ptr<ObjectBase>>& objectList,
										 const std::shared_ptr<ObjectBase>& currentObject,
										 glm::dvec3& color, double& intensity);

	public:
		glm::dvec3	m_color;
		glm::dvec3	m_location;
		double		m_intensity;
	};
}