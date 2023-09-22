#pragma once

#include "materialbase.h"

namespace RT {
	class SimpleMaterial : public MaterialBase {

	public:
		SimpleMaterial();

		virtual ~SimpleMaterial();

		virtual glm::dvec3 ComputeColor(const std::vector<std::shared_ptr<ObjectBase>>& objectList,
										const std::vector<std::shared_ptr<LightBase>>& lightList,
										const std::shared_ptr<ObjectBase>& currentObject,
										const glm::dvec3& intPoint, const glm::dvec3& localNormal,
										const glm::dvec3& localPOI, const glm::dvec2& uvCoords,
										const Ray& cameraRay) override;

		glm::dvec3 ComputeSpecular(	const std::vector<std::shared_ptr<ObjectBase>>& objectList,
									const std::vector<std::shared_ptr<LightBase>>& lightBase,
									const glm::dvec3& intPoint, const glm::dvec3& localNormal,
									const Ray& cameraRay);

	public:
		glm::dvec3 m_baseColor;

		double m_reflectivity = 0.0;
		double m_shininess = 0.0;
	};
}