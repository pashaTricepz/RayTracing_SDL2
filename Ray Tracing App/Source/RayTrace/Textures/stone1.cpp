#include "stone1.h"

RT::Texture::Stone1::Stone1()
{
	m_amplitude1 = 1.0;
	m_amplitude2 = 1.0;

	m_scale1 = 32;
	m_scale2 = 64;

	m_minValue = -2.0;
	m_maxValue = 2.0;

	m_noiseGenerator1.SetupGrid(m_scale1);
	m_noiseGenerator2.SetupGrid(m_scale2);

	auto stoneMap = std::make_shared<Texture::ColorMap>(ColorMap());
	stoneMap->SetStop(0.0, { 0.2, 0.2, 0.2, 1.0 });
	stoneMap->SetStop(0.25, { 0.8, 0.8, 0.8, 1.0 });
	stoneMap->SetStop(0.5, { 0.5, 0.5, 0.5, 1.0 });
	stoneMap->SetStop(0.75, { 0.1, 0.1, 0.1, 1.0 });
	stoneMap->SetStop(1.0, { 0.2, 0.2, 0.2, 1.0 });
	SetColorMap(stoneMap);
}

RT::Texture::Stone1::~Stone1()
{
}

glm::dvec4 RT::Texture::Stone1::GetColor(const glm::dvec2& uvCoords)
{
	glm::dvec2 inputLoc = uvCoords;
	glm::dvec2 newLoc = ApplyTransform(inputLoc);
	double newU = newLoc[0];
	double newV = newLoc[1];

	glm::dvec4 localColor(1.0, 0.0, 1.0, 1.0);

	if (m_haveColorMap) {
		double mapPosition = m_noiseGenerator1.GetValue(newU, newV) * m_amplitude1 + m_noiseGenerator2.GetValue(newU, newV) * m_amplitude2;

		mapPosition = std::clamp((mapPosition - m_minValue) / (m_maxValue - m_minValue), 0.0, 1.0);
		localColor = m_colorMap->GetColor(mapPosition);
	}
	return localColor;
}

double RT::Texture::Stone1::GetValue(const glm::dvec2& uvCoords)
{
	glm::dvec2 inputLoc = uvCoords;
	glm::dvec2 newLoc = ApplyTransform(inputLoc);
	double newU = newLoc[0];
	double newV = newLoc[1];

	double mapPosition = m_noiseGenerator1.GetValue(newU, newV) * m_amplitude1 + m_noiseGenerator2.GetValue(newU, newV) * m_amplitude2;

	mapPosition = std::clamp((mapPosition - m_minValue) / (m_maxValue - m_minValue), 0.0, 1.0);
	return mapPosition;
}

void RT::Texture::Stone1::SetColorMap(const std::shared_ptr<Texture::ColorMap>& colorMap)
{
	m_colorMap = colorMap;
	m_haveColorMap = true;
}

void RT::Texture::Stone1::SetAmplitude(double amplitude)
{
	m_amplitude1 = amplitude;
}

void RT::Texture::Stone1::SetScale(int scale)
{
	m_scale1 = scale;
	m_noiseGenerator1.SetupGrid(m_scale1);
}
