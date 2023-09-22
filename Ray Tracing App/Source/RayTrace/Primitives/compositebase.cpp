#include "compositebase.h"

RT::SHAPES::CompositeBase::CompositeBase()
{
	
}

RT::SHAPES::CompositeBase::~CompositeBase()
{

}

void RT::SHAPES::CompositeBase::AddSubShape(std::shared_ptr<RT::ObjectBase> subShape)
{
	glm::dvec2 xLim(0.0);
	glm::dvec2 yLim(0.0);
	glm::dvec2 zLim(0.0);

	subShape->GetExtents(xLim, yLim, zLim);

	if (xLim[0] < m_xLim[0]) {
		m_xLim[0] = xLim[0];
	}
	if (xLim[1] > m_xLim[1]) {
		m_xLim[1] = xLim[1];
	}

	if (yLim[0] < m_yLim[0]) {
		m_yLim[0] = yLim[0];
	}
	if (yLim[1] > m_yLim[1]) {
		m_yLim[1] = yLim[1];
	}

	if (zLim[0] < m_zLim[0]) {
		m_zLim[0] = zLim[0];
	}
	if (zLim[1] > m_zLim[1]) {
		m_zLim[1] = zLim[1];
	}

	double xSize = m_xLim[1] - m_xLim[0];
	double ySize = m_yLim[1] - m_yLim[0];
	double zSize = m_zLim[1] - m_zLim[0];

	double xCentre = m_xLim[0] + (xSize / 2.0);
	double yCentre = m_yLim[0] + (ySize / 2.0);
	double zCentre = m_zLim[0] + (zSize / 2.0);

	m_boundingBoxTransform.SetTransform({ xCentre, yCentre, zCentre }, { 0.0, 0.0, 0.0 }, { xSize / 2.0, ySize / 2.0, zSize / 2.0 });

	m_boundingBox.SetTransformMatrix(m_boundingBoxTransform);

	m_shapeList.push_back(subShape);
}

int RT::SHAPES::CompositeBase::TestIntersection(const Ray& castRay, const Ray& bckRay, glm::dvec3& worldIntPoint, double& currentDist, DATA::hitData& tempHitData)
{
	int numShapes = m_shapeList.size();
	int validShapeIndex = -1;
	DATA::hitData hitData;
	
	for (int i = 0; i < numShapes; ++i) {
		if (m_shapeList[i]->m_isVisible) {
			bool shapeTest = m_shapeList[i]->TestIntersection(bckRay, hitData);
			if (shapeTest) {
				glm::dvec3 intPoint = m_transformMatrix.Apply(hitData.poi, RT::FWDTFORM);
				double dist = glm::length(intPoint - castRay.m_point1);
				
				if (dist < currentDist) {
					currentDist = dist;
					validShapeIndex = i;
					worldIntPoint = intPoint;
					tempHitData = hitData;
				}
			}
		}
	}
	return validShapeIndex;
}

void RT::SHAPES::CompositeBase::GetExtents(glm::dvec2& xLim, glm::dvec2& yLim, glm::dvec2& zLim)
{
	std::vector<glm::dvec3> cornerPoints = ConstructCube(m_xLim[0], m_xLim[1], m_yLim[0], m_yLim[1], m_zLim[0], m_zLim[1]);

	double minX = 1e6;
	double minY = 1e6;
	double minZ = 1e6;
	double maxX = -1e6;
	double maxY = -1e6;
	double maxZ = -1e6;

	for (int i = 0; i < 8; ++i) {
		cornerPoints[i] = m_transformMatrix.Apply(cornerPoints[i], RT::FWDTFORM);

		if (cornerPoints[i][0] < minX) {
			minX = cornerPoints[i][0];
		}
		if (cornerPoints[i][0] > maxX) {
			maxX = cornerPoints[i][0];
		}

		if (cornerPoints[i][1] < minY) {
			minY = cornerPoints[i][1];
		}
		if (cornerPoints[i][1] > maxY) {
			maxY = cornerPoints[i][1];
		}

		if (cornerPoints[i][2] < minZ) {
			minZ = cornerPoints[i][2];
		}
		if (cornerPoints[i][2] > maxZ) {
			maxZ = cornerPoints[i][2];
		}
	}

	xLim[0] = minX;
	xLim[1] = maxX;
	yLim[0] = minY;
	yLim[1] = maxY;
	zLim[0] = minZ;
	zLim[1] = maxZ;
}

void RT::SHAPES::CompositeBase::UpdateBounds()
{
	m_xLim = glm::dvec2(1e6, -1e6);
	m_yLim = glm::dvec2(1e6, -1e6);
	m_zLim = glm::dvec2(1e6, -1e6);

	glm::dvec2 xLim(0.0);
	glm::dvec2 yLim(0.0);
	glm::dvec2 zLim(0.0);

	for (auto shape : m_shapeList) {
		shape->GetExtents(xLim, yLim, zLim);
		if (xLim[0] < m_xLim[0])
			m_xLim[0] = xLim[0];
		if (xLim[1] > m_xLim[1])
			m_xLim[1] = xLim[1];

		if (yLim[0] < m_yLim[0])
			m_yLim[0] = yLim[0];
		if (yLim[1] > m_yLim[1])
			m_yLim[1] = yLim[1];

		if (zLim[0] < m_zLim[0])
			m_zLim[0] = zLim[0];
		if (zLim[1] > m_zLim[1])
			m_zLim[1] = zLim[1];
	}
	double xSize = m_xLim[1] - m_xLim[0];
	double ySize = m_yLim[1] - m_yLim[0];
	double zSize = m_zLim[1] - m_zLim[0];

	double xCentre = m_xLim[0] + (xSize / 2.0);
	double yCentre = m_yLim[0] + (ySize / 2.0);
	double zCentre = m_zLim[0] + (zSize / 2.0);

	m_boundingBoxTransform.SetTransform({ xCentre, yCentre, zCentre }, { 0.0, 0.0, 0.0 }, { xSize / 2.0, ySize / 2.0, zSize / 2.0 });

	m_boundingBox.SetTransformMatrix(m_boundingBoxTransform);
}

bool RT::SHAPES::CompositeBase::TestIntersection(const Ray& castRay, DATA::hitData& hitData)
{
	if (!m_isVisible) {
		return false;
	}

	Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM);

	if (!m_useBoundingBox or m_boundingBox.TestIntersection(bckRay)) {
		glm::dvec3 worldIntPoint(0.0);
		double currentDist = 100e6;
		RT::DATA::hitData tempHitData;
		int validShapeIndex = TestIntersection(castRay, bckRay, worldIntPoint, currentDist, tempHitData);

		if (validShapeIndex > -1) {
			glm::dvec3 newNormal = glm::normalize(m_transformMatrix.ApplyNorm(tempHitData.normal));
			tempHitData.hitObject->ComputeUV(tempHitData.localPOI, hitData.uvCoords);

			hitData.poi = worldIntPoint;
			hitData.normal = newNormal;
			hitData.color = tempHitData.color;
			hitData.localPOI = tempHitData.localPOI;
			hitData.hitObject = tempHitData.hitObject;
			
			return true;
		}
		else {
			return false;
		}

	}
	else {
		return false;
	}
	return false;
}
