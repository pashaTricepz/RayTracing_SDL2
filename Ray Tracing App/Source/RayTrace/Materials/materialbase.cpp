 #include "materialbase.h"

RT::MaterialBase::MaterialBase()
{
	m_maxReflectionRays = 3;
	m_reflectionRayCount = 0;
	m_localNormal = glm::dvec3(0.0);
}

RT::MaterialBase::~MaterialBase()
{

}

glm::dvec3 RT::MaterialBase::ComputeColor(const std::vector<std::shared_ptr<ObjectBase>>& objectList, const std::vector<std::shared_ptr<LightBase>>& lightBase, const std::shared_ptr<ObjectBase>& currentObject, const glm::dvec3& intPoint, const glm::dvec3& localNormal, const glm::dvec3& localPOI, const glm::dvec2& uvCoords, const Ray& cameraRay)
{
	glm::dvec3 matColor(0.0);
	return matColor;
}

glm::dvec3 RT::MaterialBase::ComputeDiffuseColor(const std::vector<std::shared_ptr<ObjectBase>>& objectList, const std::vector<std::shared_ptr<LightBase>>& lightList, const std::shared_ptr<ObjectBase>& currentObject, const glm::dvec3& intPoint, const glm::dvec3& localNormal, const glm::dvec3& baseColor)
{
	glm::dvec3 diffuseColor(0.0);
	glm::dvec3 color(0.0);
	double intensity;

	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;

	bool validIllum = false;
	bool illumFound = false;

	for (auto currentLight : lightList) {
		validIllum = currentLight->ComputeIllumination(intPoint, localNormal, objectList, NULL, color, intensity);

		if (validIllum) {
			illumFound = true;
			red += color[0] * intensity;
			green += color[1] * intensity;
			blue += color[2] * intensity;
		}
	}

	if (illumFound) {
		diffuseColor[0] = red * baseColor[0];
		diffuseColor[1] = green * baseColor[1];
		diffuseColor[2] = blue * baseColor[2];
	}
	else {
		diffuseColor[0] = (m_ambientColor[0] * m_ambientIntensity) * baseColor[0];
		diffuseColor[1] = (m_ambientColor[1] * m_ambientIntensity) * baseColor[1];
		diffuseColor[2] = (m_ambientColor[2] * m_ambientIntensity) * baseColor[2];
	}

	return diffuseColor;
}

glm::dvec3 RT::MaterialBase::ComputeReflectionColor(const std::vector<std::shared_ptr<ObjectBase>>& objectList, const std::vector<std::shared_ptr<LightBase>>& lightList, const std::shared_ptr<ObjectBase>& currentObject, const glm::dvec3& intPoint, const glm::dvec3& localNormal, const Ray& incidentRay)
{
	glm::dvec3 reflectionColor(0.0);

	glm::dvec3 d = incidentRay.m_lab;

	glm::dvec3 reflectionVector = d - (2.0 * glm::dot(d, localNormal) * localNormal);

	glm::dvec3 startPoint = intPoint + (localNormal * 0.001);
	glm::dvec3 endPoint = startPoint + reflectionVector;
	Ray reflectionRay(startPoint, endPoint);

	std::shared_ptr<ObjectBase> closestObject;
	DATA::hitData closestHitData;
	bool intersectionFound = CastRay(reflectionRay, objectList, NULL, closestObject, closestHitData);
	
	glm::dvec3 matColor(0.0);
	if ((intersectionFound) and (m_reflectionRayCount < m_maxReflectionRays)) {
		m_reflectionRayCount++;

		if (closestHitData.hitObject->m_hasMaterial) {
			matColor = closestHitData.hitObject->m_pMaterial->ComputeColor(objectList, lightList, closestHitData.hitObject, closestHitData.poi, closestHitData.normal, closestHitData.localPOI, closestHitData.uvCoords, reflectionRay);
		}
		else {
			matColor = MaterialBase::ComputeDiffuseColor(objectList, lightList, closestHitData.hitObject, closestHitData.poi, closestHitData.normal, closestObject->m_baseColor);
		}
	}
	else {

	}
	reflectionColor = matColor;
	return reflectionColor;
}

glm::dvec3 RT::MaterialBase::ComputeSpecAndDiffuse(const std::vector<std::shared_ptr<ObjectBase>>& objectList, const std::vector<std::shared_ptr<LightBase>>& lightList, const std::shared_ptr<ObjectBase>& currentObject, const glm::dvec3& intPoint, const glm::dvec3& localNormal, const glm::dvec3& baseColor, const Ray& cameraRay)
{
	glm::dvec3 outputColor(0.0);
	glm::dvec3 diffuseColor(0.0);
	glm::dvec3 spcColor(0.0);

	double intensity;
	double specIntensity = 0.0;

	glm::dvec3 color(0.0);

	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;
	double specR = 0.0;
	double specG = 0.0;
	double specB = 0.0;

	bool validIllum = false;
	bool illumFound = false;

	for (auto currentLight : lightList) {
		validIllum = currentLight->ComputeIllumination(intPoint, localNormal, objectList, NULL, color, intensity);
		if (validIllum) {
			illumFound = true;
			red += color[0] * intensity;
			green += color[1] * intensity;
			blue += color[2] * intensity;

			if ((m_specular > 0.0) and (m_shininess > 0.0)) {
				specIntensity = 0.0;

				glm::dvec3 lightDir = glm::normalize(currentLight->m_location - intPoint);

				glm::dvec3 startPoint = intPoint + (lightDir * 0.001);
				glm::dvec3 endPoint = startPoint + lightDir;
				Ray lightRay(startPoint, endPoint);

				glm::dvec3 d = lightRay.m_lab;
				glm::dvec3 r = d - (2 * glm::dot(d, localNormal) * localNormal);

				glm::dvec3 v = glm::normalize(cameraRay.m_lab);

				double dotProduct = glm::dot(r, v);

				if (dotProduct > 0.0) {
					specIntensity = (m_specular * std::pow(dotProduct, m_shininess));
				}

				specR += currentLight->m_color[0] * specIntensity;
				specG += currentLight->m_color[1] * specIntensity;
				specB += currentLight->m_color[2] * specIntensity;
			}
		}
	}

	if (illumFound) {
		diffuseColor[0] = red * baseColor[0];
		diffuseColor[1] = green * baseColor[1];
		diffuseColor[2] = blue * baseColor[2];
		spcColor[0] = specR;
		spcColor[1] = specG;
		spcColor[2] = specB;
	}

	diffuseColor[0] = diffuseColor[0] + m_ambientColor[0] * m_ambientIntensity * baseColor[0];
	diffuseColor[1] = diffuseColor[1] + m_ambientColor[1] * m_ambientIntensity * baseColor[1];
	diffuseColor[2] = diffuseColor[2] + m_ambientColor[2] * m_ambientIntensity * baseColor[2];

	outputColor = diffuseColor + spcColor;

	return outputColor;
}

bool RT::MaterialBase::CastRay(const Ray& castRay, const std::vector<std::shared_ptr<ObjectBase>>& objectList, const std::shared_ptr<ObjectBase>& thisObject, std::shared_ptr<ObjectBase>& closestObject, RT::DATA::hitData& closestHitData)
{
	DATA::hitData hitData;

	double minDist = 1e6;
	bool intersectionFound = false;

	for (auto currentObject : objectList) {

		if (currentObject != thisObject) {
			bool validInt = currentObject->TestIntersection(castRay, hitData);

			if (validInt) {
				intersectionFound = true;

				double dist = glm::length(hitData.poi - castRay.m_point1);

				if (dist < minDist) {
					minDist = dist;
					closestObject = currentObject;
					closestHitData = hitData;
				}
			}
		}
	}
	return intersectionFound;
}

void RT::MaterialBase::AssignTexture(const std::shared_ptr<Texture::TextureBase>& inputTexture)
{
	m_textureList.push_back(inputTexture);
	m_hasTexture = true;
}

void RT::MaterialBase::AssignNormalMap(const std::shared_ptr<Normal::NormalBase>& inputNormalMap)
{
	m_normalMapList.push_back(inputNormalMap);
	m_hasNormalMap = true;
}

glm::dvec4 RT::MaterialBase::GetTextureColor(const glm::dvec2& uvCoords)
{
	glm::dvec4 outputColor(0.0);
	outputColor = m_textureList[0]->GetColor(uvCoords);
	if (m_textureList.size() > 1) {
		for (int i = 1; i < m_textureList.size(); ++i) {
			BlendColors(outputColor, m_textureList[i]->GetColor(uvCoords));
		}
	}
	return outputColor;
}

glm::dvec3 RT::MaterialBase::PerturbNormal(const glm::dvec3& normal, const glm::dvec2& uvCoords)
{
	glm::dvec3 newNormal = normal;

	for (int i = 0; i < m_normalMapList.size(); ++i) {
		newNormal = m_normalMapList[i]->ComputePerturbation(newNormal, uvCoords);
	}

	return newNormal;
}

void RT::MaterialBase::BlendColors(glm::dvec4& color1, const glm::dvec4& color2)
{
	color1 = (color2 * color2[3]) + (color1 * (1.0 - color2[3]));
}
