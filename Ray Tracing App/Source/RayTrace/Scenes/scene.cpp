#include "scene.h"

RT::Scene::Scene()
{
	SetupSceneObjects();
}

RT::Scene::~Scene()
{
}

// Function to perform the rendering.
bool RT::Scene::Render(Image& outputImage)
{

	int xSize = outputImage.GetXSize();
	int ySize = outputImage.GetYSize();

	for (int y = 0; y < ySize; ++y)
	{
		for (int x = 0; x < xSize; ++x)
		{
			glm::dvec3 pixelColor = RenderPixel(x, y, xSize, ySize);
			outputImage.SetPixel(x, y, pixelColor[0], pixelColor[1], pixelColor[2]);
		}
	}

	std::cout << std::endl;
	return true;
}

void RT::Scene::RenderTile(RT::DATA::tile* tile)
{
	glm::dvec3 pixelColor;
	for (int y = 0; y < tile->ySize; ++y) {
		for (int x = 0; x < tile->xSize; ++x) {
			pixelColor = RenderPixel(tile->x + x, tile->y + y, m_xSize, m_ySize);
			tile->rgbData[Sub2Ind(x, y, tile->xSize, tile->ySize)] = pixelColor;
		}
	}
	tile->renderComplete = true;
}

bool RT::Scene::CastRay(RT::Ray& castRay, std::shared_ptr<RT::ObjectBase>& closestObject,
	RT::DATA::hitData& closestHitData)
{
	RT::DATA::hitData hitData;
	double minDist = 1e6;
	bool intersectionFound = false;
	for (auto currentObject : m_objectList)
	{
		bool validInt = currentObject->TestIntersection(castRay, hitData);

		if (validInt)
		{
			intersectionFound = true;

			double dist = glm::length(hitData.poi - castRay.m_point1);

			if (dist < minDist)
			{
				minDist = dist;
				closestObject = currentObject;
				closestHitData = hitData;
			}
		}
	}
	return intersectionFound;
}

void RT::Scene::SetupSceneObjects()
{

}

glm::dvec3 RT::Scene::RenderPixel(int x, int y, int xSize, int ySize)
{
	std::shared_ptr<RT::ObjectBase> closestObject;
	RT::DATA::hitData closestHitData;
	RT::Ray cameraRay;
	double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
	double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);
	double minDist = 1e6;
	double maxDist = 0.0;

	glm::dvec3 outputColor(0.0);

	double normX = (static_cast<double>(x) * xFact) - 1.0;
	double normY = (static_cast<double>(y) * yFact) - 1.0;

	m_camera.GenerateRay(normX, normY, cameraRay);

	bool intersectionFound = CastRay(cameraRay, closestObject, closestHitData);

	if (intersectionFound)
	{
		if (closestHitData.hitObject->m_hasMaterial)
		{
			RT::MaterialBase::m_reflectionRayCount = 0;
			outputColor = closestHitData.hitObject->m_pMaterial->ComputeColor(m_objectList, m_lightList, closestHitData.hitObject, closestHitData.poi, closestHitData.normal, closestHitData.localPOI, closestHitData.uvCoords, cameraRay);
		}
		else
		{
			outputColor = RT::MaterialBase::ComputeDiffuseColor(m_objectList, m_lightList, closestHitData.hitObject, closestHitData.poi, closestHitData.normal, closestObject->m_baseColor);
		}
	}
	return outputColor;
}

int RT::Scene::Sub2Ind(int x, int y, int xSize, int ySize)
{
	if ((x < xSize) and (x >= 0) and (y < ySize) and (y >= 0)) {
		return (y * xSize) + x;
	}
	else {
		return -1;
	}
}
