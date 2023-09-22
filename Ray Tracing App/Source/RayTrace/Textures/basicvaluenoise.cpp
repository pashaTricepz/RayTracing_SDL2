#include "basicvaluenoise.h"

RT::Texture::BasicValNoise::BasicValNoise()
{
	m_noiseGenerator.SetupGrid(m_scale);
}

RT::Texture::BasicValNoise::~BasicValNoise()
{

}

glm::dvec4 RT::Texture::BasicValNoise::GetColor(const glm::dvec2& uvCoords)
{
	glm::dvec2 inputLoc = uvCoords;
	glm::dvec2 newLoc = ApplyTransform(inputLoc);

	double newU = newLoc[0];
	double newV = newLoc[1];

	glm::dvec4 localColor(0.0);

	if (!m_haveColorMap) {
		localColor = glm::dvec4(1.0, 0.0, 1.0, 1.0);
	}
	else {
		double mapPosition = glm::clamp(m_noiseGenerator.GetValue(newU, newV) * m_amplitude, 0.0, 1.0);
		localColor = m_colorMap->GetColor(mapPosition);
	}

	return localColor;
}

void RT::Texture::BasicValNoise::SetColorMap(const std::shared_ptr<Texture::ColorMap>& colorMap)
{
	m_colorMap = colorMap;
	m_haveColorMap = true;
}

void RT::Texture::BasicValNoise::SetAmplitude(double amplitude)
{
	m_amplitude = amplitude;
}

void RT::Texture::BasicValNoise::SetScale(int scale)
{
	m_scale = scale;
	m_noiseGenerator.SetupGrid(m_scale);
}
