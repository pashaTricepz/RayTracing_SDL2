#include "simplerefractive.h"

RT::SimpleRefractive::SimpleRefractive()
{
	m_baseColor = { 1.0, 0.0, 1.0 };
	m_reflectivity = 0.0;
	m_shininess = 0.0;
	m_translucency = 0.0;
	m_ior = 1.0;
}

RT::SimpleRefractive::~SimpleRefractive()
{
}

glm::dvec3 RT::SimpleRefractive::ComputeColor(const std::vector<std::shared_ptr<ObjectBase>>& objectList, const std::vector<std::shared_ptr<LightBase>>& lightList, const std::shared_ptr<ObjectBase>& currentObject, const glm::dvec3& intPoint, const glm::dvec3& localNormal, const glm::dvec3& localPOI, const glm::dvec2& uvCoords, const Ray& cameraRay)
{

	glm::dvec3 matColor(0.0);
	glm::dvec3 refColor(0.0);
	glm::dvec3 difColor(0.0);
	glm::dvec3 spcColor(0.0);
	glm::dvec3 trnColor(0.0);

	if (!m_hasTexture) {
		difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_baseColor);
	}
	else {
		glm::dvec3 textureColor = GetTextureColor(uvCoords);
		difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, textureColor);
	}

	if (m_reflectivity > 0.0)
		refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

	matColor = (refColor * m_reflectivity) + (difColor * (1.0 - m_reflectivity));

	if (m_translucency > 0.0) {
		trnColor = ComputeTranslucency(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);
	}

	matColor = (trnColor * m_translucency) + (matColor * (1.0 - m_translucency));

	if (m_shininess > 0.0)
		spcColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);

	matColor = matColor + spcColor;

	return matColor;
}

glm::dvec3 RT::SimpleRefractive::ComputeTranslucency(const std::vector<std::shared_ptr<ObjectBase>>& objectList, const std::vector<std::shared_ptr<LightBase>>& lightList, const std::shared_ptr<ObjectBase>& currentObject, const glm::dvec3& intPoint, const glm::dvec3& localNormal, const Ray& incidentRay)
{
	glm::dvec3 trnColor(0.0);

	glm::dvec3 p = glm::normalize(incidentRay.m_lab);

	glm::dvec3 tempNormal = localNormal;

	double r = 1.0 / m_ior;

	double c = -glm::dot(tempNormal, p);

	if (c < 0.0) {
		tempNormal = tempNormal * -1.0;
		c = -glm::dot(tempNormal, p);
	}

	glm::dvec3 refractedVector = r * p + (r * c - sqrt(1.0 - r * r * (1.0 - c * c))) * tempNormal;

	glm::dvec3 point1 = intPoint + (refractedVector * 0.01);
	glm::dvec3 point2 = intPoint + refractedVector;
	Ray refractedRay(point1, point2);

	std::shared_ptr<ObjectBase> closestObject;

	DATA::hitData closestHitData;
	DATA::hitData hitData;

	bool test = currentObject->TestIntersection(refractedRay, hitData);
	bool intersectionFound = false;

	Ray finalRay;
	if (test) {
		glm::dvec3 p2 = glm::normalize(refractedRay.m_lab);
		
		glm::dvec3 tempNormal2 = hitData.normal;

		double r2 = m_ior;
		double c2 = -glm::dot(tempNormal2, p2);

		if (c2 < 0.0) {
			tempNormal2 *= -1.0;
			c2 = -glm::dot(tempNormal2, p2);
		}
		
		glm::dvec3 refractedVector2 = r2 * p2 + (r2 * c2 - sqrt(1.0 - r2 * r2 * (1.0 - c2 * c2))) * tempNormal2;
	
		point1 = hitData.poi + (refractedVector2 * 0.01);
		point2 = hitData.poi + refractedVector2;
		Ray refractedRay2(point1, point2);

		intersectionFound = CastRay(refractedRay2, objectList, currentObject, closestObject, closestHitData);
		finalRay = refractedRay2;
	}
	else {
		intersectionFound = CastRay(refractedRay, objectList, currentObject, closestObject, closestHitData);
		finalRay = refractedRay;
	}

	glm::dvec3 matColor(0.0);

	if (intersectionFound) {
		if (closestObject->m_hasMaterial) {
			matColor = closestHitData.hitObject->m_pMaterial->ComputeColor(objectList, lightList, closestHitData.hitObject, closestHitData.poi, closestHitData.normal, closestHitData.localPOI, closestHitData.uvCoords, finalRay);
		}
		else {
			matColor = RT::MaterialBase::ComputeDiffuseColor(objectList, lightList, closestHitData.hitObject, closestHitData.poi, closestHitData.normal, closestObject->m_baseColor);
		}
	}
	else {

	}

	trnColor = matColor;
	return matColor;
}

glm::dvec3 RT::SimpleRefractive::ComputeSpecular(const std::vector<std::shared_ptr<ObjectBase>>& objectList, const std::vector<std::shared_ptr<LightBase>>& lightList, const glm::dvec3& intPoint, const glm::dvec3& localNormal, const Ray& cameraRay)
{
	glm::dvec3 spcColor(0.0);
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;

	// Loop through all of the lights in the scene.
	for (auto currentLight : lightList)
	{
		/* Check for intersections with all objects in the scene. */
		double intensity = 0.0;

		// Construct a vector pointing from the intersection point to the light.
		glm::dvec3 lightDir = glm::normalize(currentLight->m_location - intPoint);

		glm::dvec3 startPoint = intPoint + (lightDir * 0.001);
		glm::dvec3 point2 = startPoint + lightDir;
		RT::Ray lightRay(startPoint, point2);

		/* Loop through all objects in the scene to check if any
			obstruct light from this source. */
		DATA::hitData hitData;

		bool validInt = false;
		for (auto sceneObject : objectList)
		{
			validInt = sceneObject->TestIntersection(lightRay, hitData);
			if (validInt)
				break;
		}

		/* If no intersections were found, then proceed with
			computing the specular component. */
		if (!validInt)
		{
			// Compute the reflection vector.
			glm::dvec3 d = lightRay.m_lab;
			glm::dvec3 r = glm::normalize(d - (2.0 * glm::dot(d, localNormal) * localNormal));

			// Compute the dot product.
			glm::dvec3 v = glm::normalize(cameraRay.m_lab);
			double dotProduct = glm::dot(r, v);

			// Only proceed if the dot product is positive.
			if (dotProduct > 0.0)
			{
				intensity = m_reflectivity * std::pow(dotProduct, m_shininess);
			}
		}

		red += currentLight->m_color[0]* intensity;
		green += currentLight->m_color[1] * intensity;
		blue += currentLight->m_color[2] * intensity;
	}

	spcColor[0] = red;
	spcColor[1] = green;
	spcColor[2] = blue;

	return spcColor;
}
