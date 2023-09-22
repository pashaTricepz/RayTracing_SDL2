#include "sphere.h"

RT::Sphere::Sphere()
{
	m_uvMapType = RT::uvSPHERE;
	m_boundingBoxTransform.SetTransform({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 });
}

RT::Sphere::~Sphere()
{
}

bool RT::Sphere::TestIntersection(const Ray& castRay, DATA::hitData& hitData)
{
	Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM);

	glm::dvec3 vhat = bckRay.m_lab;

	double a = glm::dot(vhat, vhat);

	double b = 2.0 * glm::dot(bckRay.m_point1, vhat);

	double c = glm::dot(bckRay.m_point1, bckRay.m_point1) - 1.0;

	double intTest = (b * b) - 4.0 * a * c;

	glm::dvec3 poi(0.0);

	if (intTest > 0.0) {
		double numSQRT = sqrt(intTest);
		double t1 = (- b + numSQRT) / 2.0;
		double t2 = (- b - numSQRT) / 2.0;

		if (t1 < 0.0 and t2 < 0.0) {
			return false;
		}
		else {
			if (t1 < t2) {
				if (t1 > 0.0) {
					poi = bckRay.m_point1 + (vhat * t1);
				}
				else {
					if (t2 > 0.0) {
						poi = bckRay.m_point1 + (vhat * t2);
					}
					else {
						return false;
					}
				}
			}
			else {
				if (t2 > 0.0) {
					poi = bckRay.m_point1 + (vhat * t2);
				}
				else {
					if (t1 > 0.0) {
						poi = bckRay.m_point1 + (vhat * t1);
					}
					else {
						return false;
					}
				}
			}

			hitData.poi = m_transformMatrix.Apply(poi, FWDTFORM);

			glm::dvec3 normalVector = poi;
			hitData.normal = glm::normalize(m_transformMatrix.ApplyNorm(normalVector));

			hitData.color = m_baseColor;

			hitData.localPOI = poi;

			ComputeUV(hitData.localPOI, hitData.uvCoords);

			hitData.hitObject = this->shared_from_this();
		}
		return true;
	}
	return false;
}
