#include "marble.h"

RT::Texture::Marble::Marble()
{
	m_noiseGeneratorList.push_back(Noise::GrdNoiseGenerator());
	m_noiseGeneratorList.push_back(Noise::GrdNoiseGenerator());

	m_noiseGeneratorList[0].SetupGrid(4);
	m_noiseGeneratorList[1].SetupGrid(40);

	m_maxValue = 3.0;
	m_minValue = -3.0;

}

RT::Texture::Marble::~Marble()
{
}

glm::dvec4 RT::Texture::Marble::GetColor(const glm::dvec2& uvCoords)
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
		double mapPosition = m_sineAmplitude * sin(m_sineFrequency * glm::pi<double>() * (((newU + newV) / 2.0) 
											 + (m_noiseGeneratorList[0].GetValue(newU, newV) * m_amplitude1)
											 + (m_noiseGeneratorList[1].GetValue(newU, newV) * m_amplitude2)));

		mapPosition = std::clamp((mapPosition - m_minValue) / (m_maxValue - m_minValue), 0.0, 1.0);
		localColor = m_colorMap->GetColor(mapPosition);
	}
	return localColor;
}

double RT::Texture::Marble::GetValue(const glm::dvec2& uvCoords)
{
	return 0.0;
}

void RT::Texture::Marble::SetColorMap(const std::shared_ptr<Texture::ColorMap>& colorMap)
{
	m_colorMap = colorMap;
	m_haveColorMap = true;
}

void RT::Texture::Marble::SetAmplitude(double amplitude1, double amplitude2)
{
	m_amplitude1 = amplitude1;
	m_amplitude2 = amplitude2;
}

void RT::Texture::Marble::SetScale(int scale1, int scale2)
{
	m_scale1 = scale1;
	m_scale2 = scale2;

	m_noiseGeneratorList[0].SetupGrid(m_scale1);
	m_noiseGeneratorList[1].SetupGrid(m_scale2);
}

void RT::Texture::Marble::SetMinMax(double minValue, double maxValue)
{
	m_minValue = minValue;
	m_maxValue = maxValue;
}

void RT::Texture::Marble::SetSine(double amplitude, double frequency)
{
	m_sineAmplitude = amplitude;
	m_sineFrequency = frequency;
}
