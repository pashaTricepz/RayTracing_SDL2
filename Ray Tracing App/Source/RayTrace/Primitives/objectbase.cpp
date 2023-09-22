#include "objectbase.h"

#define EPSILON 1e-6f

RT::ObjectBase::ObjectBase()
{
	m_baseColor = glm::dvec3(0.0);
	m_tag = "unknow";
}

RT::ObjectBase::~ObjectBase()
{

}

bool RT::ObjectBase::TestIntersection(const Ray& castRay, RT::DATA::hitData& hitData)
{
	return false;
}

void RT::ObjectBase::GetExtents(glm::dvec2& xLim, glm::dvec2& yLim, glm::dvec2& zLim)
{
	std::vector<glm::dvec3> cornerPoints = ConstructCube(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	GTform combinedTransform = m_transformMatrix * m_boundingBoxTransform;

	double minX = 1e6;
	double minY = 1e6;
	double minZ = 1e6;
	double maxX = -1e6;
	double maxY = -1e6;
	double maxZ = -1e6;

	for (int i = 0; i < 8; ++i) {
		cornerPoints[i] = combinedTransform.Apply(cornerPoints[i], RT::FWDTFORM);

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

void RT::ObjectBase::GetExtents(const GTform& parentTransformMatrix, glm::dvec2& xLim, glm::dvec2& yLim, glm::dvec2& zLim)
{
	std::vector<glm::dvec3> cornerPoints = ConstructCube(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	GTform combinedTransform = parentTransformMatrix * m_transformMatrix * m_boundingBoxTransform;

	double minX = 1e6;
	double minY = 1e6;
	double minZ = 1e6;
	double maxX = -1e6;
	double maxY = -1e6;
	double maxZ = -1e6;

	for (int i = 0; i < 8; ++i) {
		cornerPoints[i] = combinedTransform.Apply(cornerPoints[i], RT::FWDTFORM);

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

std::vector<glm::dvec3> RT::ObjectBase::ConstructCube(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax)
{
	std::vector<glm::dvec3> cornerPoints(8);

	cornerPoints[0] = { xMin - m_boundingBoxPadding, yMin - m_boundingBoxPadding, zMin - m_boundingBoxPadding };
	cornerPoints[1] = { xMin - m_boundingBoxPadding, yMin - m_boundingBoxPadding, zMax + m_boundingBoxPadding };
	cornerPoints[2] = { xMax + m_boundingBoxPadding, yMin - m_boundingBoxPadding, zMax + m_boundingBoxPadding };
	cornerPoints[3] = { xMax + m_boundingBoxPadding, yMin - m_boundingBoxPadding, zMin - m_boundingBoxPadding };
	cornerPoints[4] = { xMin - m_boundingBoxPadding, yMax + m_boundingBoxPadding, zMin - m_boundingBoxPadding };
	cornerPoints[5] = { xMin - m_boundingBoxPadding, yMax + m_boundingBoxPadding, zMax + m_boundingBoxPadding };
	cornerPoints[6] = { xMax + m_boundingBoxPadding, yMax + m_boundingBoxPadding, zMax + m_boundingBoxPadding };
	cornerPoints[7] = { xMax + m_boundingBoxPadding, yMax + m_boundingBoxPadding, zMin - m_boundingBoxPadding };

	return cornerPoints;
}

void RT::ObjectBase::SetTransformMatrix(const GTform& transformMatrix)
{
	m_transformMatrix = transformMatrix;
}

RT::GTform RT::ObjectBase::GetTransformMatrix()
{
	return m_transformMatrix;
}

bool RT::ObjectBase::CloseEnough(const double f1, const double f2)
{
	return fabs(f1-f2) < EPSILON;
}

bool RT::ObjectBase::AssignMaterial(const std::shared_ptr<MaterialBase>& objectMaterial)
{
	m_pMaterial = objectMaterial;
	m_hasMaterial = true;
	return m_hasMaterial;
}

void RT::ObjectBase::ComputeUV(const glm::dvec3& localPOI, glm::dvec2& uvCoords)
{
#if 1
	switch (m_uvMapType)
	{
		case RT::uvSPHERE:
		{
			// Spherical projection.
			double x = localPOI[0];
			double y = localPOI[1];
			double z = localPOI[2];
			double u = atan2(y, x) / M_PI;
			double v = 2.0 * (atan2(sqrt(x * x + y * y), z) / M_PI) - 1.0;
			uvCoords[0] = u;
			uvCoords[1] = v;
			break;
		}
		case RT::uvPLANE:
		{
			// Plane projection.
			uvCoords[0] = localPOI[0];
			uvCoords[1] = localPOI[1];
			break;
		}
		case RT::uvCYLINDER:
		{
			// Cylinder projection.
			double x = localPOI[0];
			double y = localPOI[1];
			double z = localPOI[2];
			double u = atan2(y, x) / M_PI;
			double v = -z;
			uvCoords[0] = u;
			uvCoords[1] = v;
			break;
		}
		case RT::uvBOX:
		{
			// Box projection.
			double x = localPOI[0];
			double y = localPOI[1];
			double z = localPOI[2];
			double u = 0.0;
			double v = 0.0;


			if (CloseEnough(x, -1.0))
			{
				// Left face.
				u = -(y * 0.25) - 0.75;
				v = -z * 0.25;

			}
			if (CloseEnough(x, 1.0))
			{
				// Right face.
				u = (y * 0.25) + 0.25;
				v = -z * 0.25;

			}
			if (CloseEnough(y, -1.0))
			{
				// Front face.
				u = (x * 0.25) - 0.25;
				v = (-z * 0.25) - 0.5;

			}
			if (CloseEnough(y, 1.0))
			{
				// Back face.
				u = -(x * 0.25) - 0.25;;
				v = (-z * 0.25) + 0.5;

			}
			if (CloseEnough(z, -1.0))
			{
				// Top face.
				u = (x * 0.25) + 0.75;
				v = y * 0.25;

			}
			if (CloseEnough(z, 1.0))
			{
				// Bottom face.
				u = (x * 0.25) - 0.25;
				v = y * 0.25;

			}
			else
			{
				// The object isn't a box...

			}

			uvCoords[0] = u;
			uvCoords[1] = v;

			break;
		}
	}
#else
	double x = localPOI[0];
	double y = localPOI[1];
	double z = localPOI[2];
	double u = 0.0;
	double v = 0.0;
	switch (m_uvMapType) {
		case RT::uvSPHERE: {
			u = atan2(y, x) / M_PI;
			v = 2.0 * (atan2(sqrt(x * x + y * y), z) / M_PI) - 1.0;
			break;
		}
		case RT::uvPLANE: {
			u = localPOI[0];
			v = localPOI[1];
			break;
		}
		case RT::uvCYLINDER: {
			u = atan2(y, x) / M_PI;
			v = -z;
			break;
		}
		case RT::uvBOX: {
			u = 0.0;
			v = 0.0;

			glm::dmat3x3 uvTransform(1.0);

			if (CloseEnough(x, -1.0)) {
				//left face
				u = -(y * 0.25) - 0.75;
				v = -z * 0.25;
			}
			else if (CloseEnough(x, 1.0)) {
				//right face
				u = (y * 0.25) + 0.25;
				v = -z * 0.25;
			}
			else if (CloseEnough(y, -1.0)) {
				//front face
				u = (x * 0.25) - 0.25;
				v = (-z * 0.25) - 0.5;
			}
			else if (CloseEnough(y, 1.0)) {
				//back face
				u = -(x * 0.25) - 0.25;
				v = (-z * 0.25) + 0.5;
			}
			else if (CloseEnough(z, -1.0)) {
				//top face
				u = (x * 0.25) + 0.75;
				v = y * 0.25;
			}
			else if (CloseEnough(z, 1.0)) {
				//bottom face
				u = (x * 0.25) - 0.25;
				v = y * 0.25;
			}
			else {

			}
			break;
		}
	};
	uvCoords[0] = u;
	uvCoords[1] = v;
#endif
}

