#include "cube.h"

RT::RM::Cube::Cube()
{
	std::function<double(glm::dvec3*, glm::dvec3*)> f = [=](glm::dvec3* location, glm::dvec3* parms) {
		return this->ObjectFcn(location, parms);
	};

	SetObjectFcn(f);

	m_boundingBox.SetTransformMatrix(RT::GTform({ 0.0, 0.0, 0.0, }, { 0.0, 0.0, 0.0 }, { 1.2, 1.2, 1.2 }));
}

RT::RM::Cube::~Cube()
{
}

double RT::RM::Cube::ObjectFcn(glm::dvec3* location, glm::dvec3* parms)
{
	glm::dvec3 center(0.0);
	glm::dvec3 intParms(1.0);

	return RM::SDF::Box(*location, center, intParms);
}
