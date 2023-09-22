#include "camera.h"
#include "ray.h"

RT::Camera::Camera()
{
	m_cameraPosition	= { 0.0, -10.0, 0.0 };
	m_cameraLookAt		= { 0.0, 0.0, 0.0 };
	m_cameraUp			= { 0.0, 0.0, 1.0 };
	m_cameraLength		= 1.0;
	m_cameraHorSize		= 1.0;
	m_cameraAspectRatio = 1.0;
	m_alignmentVector = glm::dvec3(0.0);
	m_projectiomScreenCentre = glm::dvec3(0.0);
	m_projectiomScreenU = glm::dvec3(0.0);
	m_projectiomScreenV = glm::dvec3(0.0);

}

void RT::Camera::SetPosition(glm::dvec3&& newPosition)
{
	m_cameraPosition = newPosition;
}

void RT::Camera::SetLookAt(glm::dvec3&& newLookAt)
{
	m_cameraLookAt = newLookAt;
}

void RT::Camera::SetUp(glm::dvec3&& upVector)
{
	m_cameraUp = upVector;
}

void RT::Camera::SetLength(double newLength)
{
	m_cameraLength = newLength;
}

void RT::Camera::SetHorzSize(double newSize)
{
	m_cameraHorSize = newSize;
}

void RT::Camera::SetAspect(double newAspect)
{
	m_cameraAspectRatio = newAspect;
}

glm::dvec3 RT::Camera::GetPosition()
{
	return m_cameraPosition;
}

glm::dvec3 RT::Camera::GetLookAt()
{
	return m_cameraLookAt;
}

glm::dvec3 RT::Camera::GetUp()
{
	return m_cameraUp;
}

glm::dvec3 RT::Camera::GetU()
{
	return m_projectiomScreenU;
}

glm::dvec3 RT::Camera::GetY()
{
	return m_projectiomScreenV;
}

glm::dvec3 RT::Camera::GetScreenCentre()
{
	return m_projectiomScreenCentre;
}

double RT::Camera::GetLength()
{
	return m_cameraLength;
}

double RT::Camera::GetHorzSize()
{
	return m_cameraHorSize;
}

double RT::Camera::GetAspect()
{
	return m_cameraAspectRatio;
}

bool RT::Camera::GenerateRay(double proScreenX, double proScreenY, Ray& cameraRay)
{
	glm::dvec3 screenWorldPart1 = m_projectiomScreenCentre + (m_projectiomScreenU * proScreenX);
	glm::dvec3 screenWorldCoordinate = screenWorldPart1 + (m_projectiomScreenV * proScreenY);

	cameraRay.m_point1 = m_cameraPosition;
	cameraRay.m_point2 = screenWorldCoordinate;

	cameraRay.m_lab = screenWorldCoordinate - m_cameraPosition;

	return true;
}

void RT::Camera::UpdateCameraGeometry()
{
	m_alignmentVector = glm::normalize(m_cameraLookAt - m_cameraPosition);

	m_projectiomScreenU = glm::normalize(glm::cross(m_alignmentVector, m_cameraUp));
	m_projectiomScreenV = glm::normalize(glm::cross(m_projectiomScreenU, m_alignmentVector));

	m_projectiomScreenCentre = m_cameraPosition + (m_cameraLength * m_alignmentVector);

	m_projectiomScreenU = m_projectiomScreenU * m_cameraHorSize;
	m_projectiomScreenV = m_projectiomScreenV * (m_cameraHorSize / m_cameraAspectRatio);
}
