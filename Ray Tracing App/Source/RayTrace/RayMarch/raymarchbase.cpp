#include "raymarchbase.h"

RT::RM::RayMarchBase::RayMarchBase()
{
	m_epsilon = 1e-12;
	m_maxSteps = 100;
}

RT::RM::RayMarchBase::~RayMarchBase()
{
}

bool RT::RM::RayMarchBase::TestIntersection(const Ray& castRay, DATA::hitData& hitData)
{
	if (m_haveObjectFcn) {
		if (!m_isVisible) {
			return false;
		}

		RT::Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM);

		if (m_boundingBox.TestIntersection(bckRay)) {
			glm::dvec3 vhat = glm::normalize(bckRay.m_lab);

			glm::dvec3 currentLoc = bckRay.m_point1;
			int stepCount = 0;
			double dist = EvaluateSDF(&currentLoc, &m_parms);

			while ((dist > m_epsilon) and (stepCount < m_maxSteps)) {
				currentLoc += vhat * dist;

				dist = EvaluateSDF(&currentLoc, &m_parms);

				if (dist > 1e6) {
					stepCount = m_maxSteps;
					break;
				}
				stepCount++;
			}

			if (stepCount == m_maxSteps) {
				return false;
			}

			hitData.poi = m_transformMatrix.Apply(currentLoc, RT::FWDTFORM);

			glm::dvec3 surfaceNormal(0.0);

			glm::dvec3 normalLoc = currentLoc - (vhat * 0.01);

			glm::dvec3 x1 = normalLoc - m_xDisp;
			glm::dvec3 x2 = normalLoc + m_xDisp;
			glm::dvec3 y1 = normalLoc - m_yDisp;
			glm::dvec3 y2 = normalLoc + m_yDisp;
			glm::dvec3 z1 = normalLoc - m_zDisp;
			glm::dvec3 z2 = normalLoc + m_zDisp;

			surfaceNormal[0] = EvaluateSDF(&x2, &m_parms) - EvaluateSDF(&x1, &m_parms);
			surfaceNormal[1] = EvaluateSDF(&y2, &m_parms) - EvaluateSDF(&y1, &m_parms);
			surfaceNormal[2] = EvaluateSDF(&z2, &m_parms) - EvaluateSDF(&z1, &m_parms);

			surfaceNormal = glm::normalize(surfaceNormal);

			hitData.normal = m_transformMatrix.ApplyNorm(surfaceNormal);

			hitData.color = m_baseColor;

			hitData.hitObject = this->shared_from_this();

			hitData.localPOI = currentLoc;

			ComputeUV(currentLoc, hitData.uvCoords);

			return true;
		}
	}
	return false;
}

void RT::RM::RayMarchBase::SetObjectFcn(std::function<double(glm::dvec3*, glm::dvec3*)> objectFcn)
{
	m_objectFcn = objectFcn;
	m_haveObjectFcn = true;
}

double RT::RM::RayMarchBase::EvaluateSDF(glm::dvec3* location, glm::dvec3* parms)
{
	return m_objectFcn(location, parms);
}
