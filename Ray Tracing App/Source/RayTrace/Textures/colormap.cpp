#include "colormap.h"
#include <iostream>

RT::Texture::ColorMap::ColorMap()
{

}

RT::Texture::ColorMap::~ColorMap()
{
}

void RT::Texture::ColorMap::SetStop(double position, const glm::dvec4& value)
{
	
	m_stopPositions.push_back(position);
	m_stopValues.push_back(value);
}

glm::dvec4 RT::Texture::ColorMap::GetColor(double position)
{
	int numStops = m_stopPositions.size();
	int firstStop = 0;
	int secondStop = 0;
	double diff = 2.0;

	for (int i = 0; i < numStops; ++i) {
		double t = m_stopPositions[i] - position;
		if (fabs(t) < diff) {
			diff = fabs(t);
			firstStop = i;
			if (t < 0.0) {
				secondStop = std::min(numStops, i + 1);
			}
			else if (t > 0.0) {
				secondStop = std::max(i - 1, 0);
			}
			else {
				secondStop = i;
			}
		}

	}
	if (firstStop == secondStop) {
		return m_stopValues[firstStop];
	}

	if (secondStop < firstStop) {
		std::swap(secondStop, firstStop);
	}

	double x = position;
	double x0 = m_stopPositions[firstStop];
	double x1 = m_stopPositions[secondStop];
	
	return m_stopValues[firstStop] + (x - x0) * ((m_stopValues[secondStop] - m_stopValues[firstStop]) * (1.0 / (x1 - x0)));
}
