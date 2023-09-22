#include "torus.h"

RT::RM::Torus::Torus()
{
	std::function<double(glm::dvec3*, glm::dvec3*)> f = [=](glm::dvec3* location, glm::dvec3* parms) {
		return this->ObjectFcn(location, parms);
	};

	SetObjectFcn(f);

	m_boundingBox.SetTransformMatrix(GTform({ 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, { 1.3, 1.3, 1.3 }));
}

RT::RM::Torus::~Torus()
{
	
}

void RT::RM::Torus::SetRadii(double r1, double r2)
{
	m_parms[0] = r1;
	m_parms[1] = r2;
	UpdateBounds();
}

double RT::RM::Torus::ObjectFcn(glm::dvec3* location, glm::dvec3* parms)
{
	glm::dvec3 center(0.0);
	return RT::RM::SDF::Torus(*location, center, *parms);
}

void RT::RM::Torus::UpdateBounds()
{
	m_boundingBox.SetTransformMatrix(GTform({ 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, 
							{ m_r1 + m_r2 + 0.3, m_r1 + m_r2 + 0.3, m_r2 + 0.2 }));
}
