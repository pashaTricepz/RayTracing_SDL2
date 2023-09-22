#include "gradient.h"

RT::Texture::Gradient::Gradient()
{

}

RT::Texture::Gradient::~Gradient()
{

}

glm::dvec4 RT::Texture::Gradient::GetColor(const glm::dvec2& uvCoords)
{
	glm::dvec2 inputLoc = uvCoords;
	glm::dvec2 newLoc = ApplyTransform(inputLoc);
	double newU = std::min((newLoc[1] + 1) / 2, 1.0);
	return m_colorMap.GetColor(newU);
}

double RT::Texture::Gradient::GetValue(const glm::dvec2& uvCoords)
{
	glm::dvec2 inputLoc = uvCoords;
	glm::dvec2 newLoc = ApplyTransform(inputLoc);
	return std::min((newLoc[0] + 1) / 2, 1.0);
}

void RT::Texture::Gradient::SetStop(double position, const glm::dvec4& value)
{
	m_colorMap.SetStop(position, value);
}
