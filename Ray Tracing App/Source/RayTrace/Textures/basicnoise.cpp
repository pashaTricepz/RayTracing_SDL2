#include "basicnoise.h"

RT::Texture::BasicNoise::BasicNoise()
{
	m_noiseGenerator.SetupGrid(m_scale);
}

RT::Texture::BasicNoise::~BasicNoise()
{
}

glm::dvec4 RT::Texture::BasicNoise::GetColor(const glm::dvec2& uvCoords)
{
	glm::dvec2 inputLoc = uvCoords;
	glm::dvec2 newLoc = ApplyTransform(inputLoc);

	double newU = newLoc[0];
	double newV = newLoc[1];

	glm::dvec4 localColor(0.0);
	if (!m_haveColorMap) {
		localColor = { 1.0, 0.0, 1.0, 1.0 };
	}
	else {
		double mapPosition = std::clamp(m_noiseGenerator.GetValue(newU, newV) * m_amplitude, 0.0, 1.0);
		localColor = m_colorMap->GetColor(mapPosition);
	}
	return localColor;
}

void RT::Texture::BasicNoise::SetColorMap(const std::shared_ptr<Texture::ColorMap>& colorMap)
{
	m_colorMap = colorMap;
	m_haveColorMap = true;
}

void RT::Texture::BasicNoise::SetAmplitude(double amplitude)
{
	m_amplitude = amplitude;
}

void RT::Texture::BasicNoise::SetScale(int scale)
{
	m_scale = scale;
	m_noiseGenerator.SetupGrid(m_scale);
}
