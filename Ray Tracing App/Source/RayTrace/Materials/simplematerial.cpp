#include "simplematerial.h"

RT::SimpleMaterial::SimpleMaterial()
{
	m_baseColor = glm::dvec3(1.0, 0.0, 1.0);
}

RT::SimpleMaterial::~SimpleMaterial()
{
}

glm::dvec3 RT::SimpleMaterial::ComputeColor(const std::vector<std::shared_ptr<ObjectBase>>& objectList, const std::vector<std::shared_ptr<LightBase>>& lightList, const std::shared_ptr<ObjectBase>& currentObject, const glm::dvec3& intPoint, const glm::dvec3& localNormal,const glm::dvec3& localPOI, const glm::dvec2& uvCoords, const Ray& cameraRay)
{

	glm::dvec3 matColor(0.0);
	glm::dvec3 refColor(0.0);
	glm::dvec3 diffColor(0.0);
	glm::dvec3 spcColor(0.0);

	glm::dvec3 newNormal = localNormal;
	if (m_hasNormalMap) {
		newNormal = PerturbNormal(newNormal, uvCoords);
	}

	m_localNormal = newNormal;

	if (!m_hasTexture) {
		diffColor = ComputeSpecAndDiffuse(objectList, lightList, currentObject, intPoint, newNormal, m_baseColor, cameraRay);
	}
	else {
		glm::dvec3 textureColor = GetTextureColor(uvCoords);
		diffColor = ComputeSpecAndDiffuse(objectList, lightList, currentObject, intPoint, newNormal, textureColor, cameraRay);
	}

	if (m_reflectivity > 0.0) {
		refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, newNormal, cameraRay);
	}

	matColor = (refColor * m_reflectivity) + (diffColor * (1 - m_reflectivity));

	/*if (m_shininess > 0.0) {
		spcColor = ComputeSpecular(objectList, lightList, intPoint, newNormal, cameraRay);
	}
	matColor += spcColor;*/

	return matColor;
}

glm::dvec3 RT::SimpleMaterial::ComputeSpecular(const std::vector<std::shared_ptr<ObjectBase>>& objectList, const std::vector<std::shared_ptr<LightBase>>& lightList, const glm::dvec3& intPoint, const glm::dvec3& localNormal, const Ray& cameraRay)
{
	glm::dvec3 spcColor(0.0);

	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;

	for (auto currentLight : lightList) {
		double intensity = 0.0;

		glm::dvec3 lightDir = glm::normalize(currentLight->m_location - intPoint);

		glm::dvec3 startPoint = intPoint + (lightDir * 0.001);

		glm::dvec3 endPoint = startPoint + lightDir;

		Ray lightRay(startPoint, endPoint);

		DATA::hitData htiData;

		bool validInt = false;

		for (auto sceneObject : objectList) {
			validInt = sceneObject->TestIntersection(lightRay, htiData);
			if (validInt) {
				break;
			}
		}

		if (!validInt) {
			glm::dvec3 d = lightRay.m_lab;
			glm::dvec3 r = glm::normalize(d - (2 * glm::dot(d, localNormal) * localNormal));

			glm::dvec3 v = glm::normalize(cameraRay.m_lab);

			double dotProduct = glm::dot(r, v);


			if (dotProduct > 0.0) {
				intensity = m_reflectivity * std::pow(dotProduct, m_shininess);
			}
		}

		red += currentLight->m_color[0] * intensity;
		green += currentLight->m_color[1] * intensity;
		blue += currentLight->m_color[2] * intensity;
	}

	spcColor[0] = red;
	spcColor[1] = green;
	spcColor[2] = blue;

	return spcColor;
}
