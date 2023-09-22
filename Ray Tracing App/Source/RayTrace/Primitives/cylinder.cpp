#include "cylinder.h"

RT::Cylinder::Cylinder()
{
	m_uvMapType = RT::uvCYLINDER;

	m_boundingBoxTransform.SetTransform({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 });
}

RT::Cylinder::~Cylinder()
{
}

bool RT::Cylinder::TestIntersection(const Ray& castRay, DATA::hitData& hitData)
{
	if (!m_isVisible) {
		return false;
	}

	Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM);

	glm::dvec3 v = glm::normalize(bckRay.m_lab);

	glm::dvec3 p = bckRay.m_point1;

	double a = v[0] * v[0] + v[1] * v[1];
	double b = 2.0 * (p[0] * v[0] + p[1] * v[1]);
	double c = p[0] * p[0] + p[1] * p[1] - 1.0;

	double numSQRT = sqrt(b * b - 4 * a * c);

	std::array<glm::dvec3, 4> poi = { glm::dvec3(0.0), glm::dvec3(0.0), glm::dvec3(0.0), glm::dvec3(0.0) };
	std::array<double, 4> t = { 0.0, 0.0, 0.0, 0.0 };
	bool t1Valid, t2Valid, t3Valid, t4Valid;

	if (numSQRT > 0.0) {
		t[0] = (-b + numSQRT) / (2 * a);
		t[1] = (-b - numSQRT) / (2 * a);

		poi[0] = bckRay.m_point1 + (v * t[0]);
		poi[1] = bckRay.m_point1 + (v * t[1]);

		if ((t[0] > 0.0) && ((fabs(poi[0][2])) < 1.0)) {
			t1Valid = true;
		}
		else {
			t1Valid = false;
			t[0] = 100e6;
		}

		if ((t[1] > 0.0) && ((fabs(poi[1][2])) < 1.0)) {
			t2Valid = true;
		}
		else {
			t2Valid = false;
			t[1] = 100e6;
		}
	}
	else {
		t1Valid = false;
		t2Valid = false;
		t[0] = 100e6;
		t[1] = 100e6;
	}

	if (CloseEnough(v[2], 0.0)) {
		t3Valid = false;
		t4Valid = false;
		t[2] = 100e6;
		t[3] = 100e6;
	}
	else {
		t[2] = (bckRay.m_point1[2] - 1.0) / -v[2];
		t[3] = (bckRay.m_point1[2] + 1.0) / -v[2];

		poi[2] = bckRay.m_point1 + t[2] * v;
		poi[3] = bckRay.m_point1 + t[3] * v;

		if ((t[2] > 0.0) && (sqrt(poi[2][0] * poi[2][0] + poi[2][1] * poi[2][1]) < 1.0)) {
			t3Valid = true;
		}
		else {
			t3Valid = false;
			t[2] = 100e6;
		}

		if ((t[3] > 0.0) && (sqrt(poi[3][0] * poi[3][0] + poi[3][1] * poi[3][1]) < 1.0)) {
			t4Valid = true;
		}
		else {
			t4Valid = false;
			t[3] = 100e6;
		}
	}

	if ((!t1Valid) && (!t2Valid) && (!t3Valid) && (!t4Valid)) {
		return false;
	}

	int minIndex = 0;
	double minValue = 10e6;

	for (int i = 0; i < 4; ++i) {
		if (t[i] < minValue) {
			minValue = t[i];
			minIndex = i;
		}
	}

	glm::dvec3 validPOI = poi[minIndex];

	if (minIndex < 2) {
		hitData.poi = m_transformMatrix.Apply(validPOI, RT::FWDTFORM);

		glm::dvec3 orgNormal(0.0);
		glm::dvec3 localOgrigin(0.0);
		glm::dvec3 globalOgrigin = m_transformMatrix.Apply(localOgrigin, RT::FWDTFORM);

		orgNormal[0] = validPOI[0];
		orgNormal[1] = validPOI[1];
		orgNormal[2] = 0.0;

		hitData.normal = glm::normalize(m_transformMatrix.ApplyNorm(orgNormal));

		hitData.color = m_baseColor;

		hitData.localPOI = validPOI;

		ComputeUV(validPOI, hitData.uvCoords);

		hitData.hitObject = this->shared_from_this();

		return true;
	}
	else {
		if (!CloseEnough(v[2], 0.0)) {
			if (sqrt(validPOI[0] * validPOI[0] + validPOI[1] * validPOI[1]) < 1.0) {
				hitData.poi = m_transformMatrix.Apply(validPOI, RT::FWDTFORM);

				glm::dvec3 normalVector(0.0, 0.0, 0.0 + validPOI[2]);
				hitData.normal = glm::normalize(m_transformMatrix.ApplyNorm(normalVector));

				hitData.color = m_baseColor;

				hitData.localPOI = validPOI;

				ComputeUV(hitData.poi, hitData.uvCoords);
				hitData.hitObject = this->shared_from_this();

				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	return false;
}
