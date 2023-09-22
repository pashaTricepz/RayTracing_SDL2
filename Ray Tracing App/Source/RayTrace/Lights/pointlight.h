#pragma once

#include "lightbase.h"

namespace RT {
	class PointLight : public LightBase {
	public:
		PointLight();

		virtual ~PointLight() override;
			
		virtual bool ComputeIllumination(const glm::dvec3& intPoint, const glm::dvec3& localNormal,
			const std::vector<std::shared_ptr<ObjectBase>>& objectList,
			const std::shared_ptr<ObjectBase>& currentObject,
			glm::dvec3& color, double& intensity) override;
	};
}