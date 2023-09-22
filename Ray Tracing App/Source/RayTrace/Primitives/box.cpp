#include "box.h"

RT::Box::Box()
{
	m_uvMapType = RT::uvBOX;
	m_boundingBoxTransform.SetTransform({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 });
}

RT::Box::~Box()
{

}

bool RT::Box::TestIntersection(const Ray& castRay, DATA::hitData& hitData)
{
	if (!m_isVisible)
		return false;

	Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM);

	std::array<double, 6> t;
	std::array<double, 6> u;
	std::array<double, 6> v;

	double ax = bckRay.m_point1[0];
	double ay = bckRay.m_point1[1];
	double az = bckRay.m_point1[2];

	glm::dvec3 k = bckRay.m_lab;

	double kx = k[0];
	double ky = k[1];
	double kz = k[2];

	//top and bottom
	if (!CloseEnough(kz, 0.0)) {
		t[0] = (az - 1.0) / -kz;
		t[1] = (az + 1.0) / -kz;
		u[0] = ax + kx * t[0];
		v[0] = ay + ky * t[0];
		u[1] = ax + kx * t[1];
		v[1] = ay + ky * t[1];
	}
	else {
		t[0] = 100e6;
		t[1] = 100e6;
		u[0] = 0.0;
		v[0] = 0.0;
		u[1] = 0.0;
		v[1] = 0.0;
	}

	//left and right
	if (!CloseEnough(kx, 0.0)) {
		t[2] = (ax + 1.0) / -kx;
		t[3] = (ax - 1.0) / -kx;
		u[2] = az + kz * t[2];
		v[2] = ay + ky * t[2];
		u[3] = az + kz * t[3];
		v[3] = ay + ky * t[3];
	}
	else {
		t[2] = 100e6;
		t[3] = 100e6;
		u[2] = 0.0;
		v[2] = 0.0;
		u[3] = 0.0;
		v[3] = 0.0;
	}

	//front and back
	if (!CloseEnough(ky, 0.0)) {
		t[4] = (ay + 1.0) / -ky;
		t[5] = (ay - 1.0) / -ky;
		u[4] = ax + kx * t[4];
		v[4] = az + kz * t[4];
		u[5] = ax + kx * t[5];
		v[5] = az + kz * t[5];
	}
	else {
		t[4] = 100e6;
		t[5] = 100e6;
		u[4] = 0.0;
		v[4] = 0.0;
		u[5] = 0.0;
		v[5] = 0.0;
	}


	double finalU = 0.0;
	double finalV = 0.0;
	double finalT = 100e6;
	int finalIndex = 0;
	bool validIntersection = false;

	for (int i = 0; i < 6; ++i) {
		if ((t[i] < finalT) and (t[i] > 0.0) and (abs(u[i]) <= 1.0) and (abs(v[i]) <= 1.0)) {
			finalT = t[i];
			finalIndex = i;
			finalU = u[i];
			finalV = v[i];
			validIntersection = true;
		}
	}

	if (validIntersection) {
		glm::dvec3 poi = bckRay.m_point1 + finalT * k;

		glm::dvec3 normalVector(0.0);

		switch (finalIndex) {
			case 0: 
				normalVector = { 0.0, 0.0, 1.0 }; //down
				break;
			case 1:
				normalVector = { 0.0, 0.0, -1.0 }; // up
				break;
			case 2:
				normalVector = { -1.0, 0.0, 0.0 }; // left
				break;
			case 3:
				normalVector = { 1.0, 0.0, 0.0 }; // right
				break;
			case 4: 
				normalVector = { 0.0, -1.0, 0.0 }; // backwards(towards the camera)
				break;
			case 5:
				normalVector = { 0.0, 1.0, 0.0 }; // forwards(away from the camera)
				break;
		}

		hitData.poi = m_transformMatrix.Apply(poi, RT::FWDTFORM);

		hitData.normal = glm::normalize(m_transformMatrix.ApplyNorm(normalVector));

		hitData.color = m_baseColor;

		hitData.localPOI = poi;

		ComputeUV(poi, hitData.uvCoords);

		hitData.hitObject = this->shared_from_this();

		return true;
	}
	else {
		return false;
	}

}

bool RT::Box::TestIntersection(const Ray& castRay)
{
	if (!m_isVisible) {
		return false;
	}

	RT::Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM);

	std::array<double, 6> t {100e6, 100e6, 100e6, 100e6, 100e6, 100e6};
	std::array<double, 6> u {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	std::array<double, 6> v {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

	double ax = bckRay.m_point1[0];
	double ay = bckRay.m_point1[1];
	double az = bckRay.m_point1[2];

	glm::dvec3 k = bckRay.m_lab;
	double kx = k[0];
	double ky = k[1];
	double kz = k[2];

	//top and bottom
	if (!CloseEnough(kz, 0.0)) {
		t[0] = (az - 1.0) / -kz;
		t[1] = (az + 1.0) / -kz;
		u[0] = ax + kx * t[0];
		v[0] = ay + ky * t[0];
		u[1] = ax + kx * t[1];
		v[1] = ay + ky * t[1];
	}
	else {
		t[0] = 100e6;
		t[1] = 100e6;
		u[0] = 0.0;
		v[0] = 0.0;
		u[1] = 0.0;
		v[1] = 0.0;
	}

	//left and right
	if (!CloseEnough(kx, 0.0)) {
		t[2] = (ax + 1.0) / -kx;
		t[3] = (ax - 1.0) / -kx;
		u[2] = az + kz * t[2];
		v[2] = ay + ky * t[2];
		u[3] = az + kz * t[3];
		v[3] = ay + ky * t[3];
	}
	else {
		t[2] = 100e6;
		t[3] = 100e6;
		u[2] = 0.0;
		v[2] = 0.0;
		u[3] = 0.0;
		v[3] = 0.0;
	}

	//front and back
	if (!CloseEnough(ky, 0.0)) {
		t[4] = (ay + 1.0) / -ky;
		t[5] = (ay - 1.0) / -ky;
		u[4] = ax + kx * t[4];
		v[4] = az + kz * t[4];
		u[5] = ax + kx * t[5];
		v[5] = az + kz * t[5];
	}
	else {
		t[4] = 100e6;
		t[5] = 100e6;
		u[4] = 0.0;
		v[4] = 0.0;
		u[5] = 0.0;
		v[5] = 0.0;
	}

	bool validIntersection = false;

	/*for (int i = 0; i < 6; ++i) {
		if ((t[i] < 100e6) and (t[i] > 0.0) and (abs(u[i]) <= 1.0) and (abs(v[i]) <= 1.0)) {
			validIntersection = true;
			return validIntersection;
		}
	}*/

	int i = 0;
	while ((i < 6) && (!validIntersection))
	{
		if ((t[i] < 100e6) && (t[i] > 0.0) && (abs(u[i]) <= 1.0) && (abs(v[i]) <= 1.0))
		{
			validIntersection = true;
			break;
		}

		i++;
	}
	return validIntersection;
}
