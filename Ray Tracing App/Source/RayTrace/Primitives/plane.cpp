#include "plane.h"

RT::Plane::Plane()
{
	m_uvMapType = RT::uvPLANE;
	m_boundingBoxTransform.SetTransform({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.01 });
}

RT::Plane::~Plane()
{
}

bool RT::Plane::TestIntersection(const Ray& castRay, DATA::hitData& hitData)
{
	if (!m_isVisible)
		return false;

	RT::Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM);

	glm::dvec3 k = bckRay.m_lab;
	//glm::dvec3 k = glm::normalize(bckRay.m_lab);

	if (CloseEnough(k[2], 0.0)) {
		return false;
	}

	double t = bckRay.m_point1[2] / -k[2];

	if (t <= 0.0) {
		return false;
	}

	double u = bckRay.m_point1[0] + (k[0] * t);
	double v = bckRay.m_point1[1] + (k[1] * t);

	if (!((abs(u) < 1.0) && (abs(v) < 1.0))) {
		return false;
	}

	glm::dvec3 poi = bckRay.m_point1 + t * k;

	hitData.poi = m_transformMatrix.Apply(poi, RT::FWDTFORM);

	glm::dvec3 normalVector(0.0, 0.0, -1.0);
	hitData.normal = glm::normalize(m_transformMatrix.ApplyNorm(normalVector));

	hitData.localPOI = poi;
	// Return the base color.
	hitData.color = m_baseColor;

	ComputeUV(poi, hitData.uvCoords);

	hitData.hitObject = this->shared_from_this();

	return true;
} 
