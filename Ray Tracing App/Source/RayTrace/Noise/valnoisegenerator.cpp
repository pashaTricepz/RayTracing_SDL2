#include "valnoisegenerator.h"

RT::Noise::ValNoiseGenerator::ValNoiseGenerator()
{
	SetupGrid(2);
}

RT::Noise::ValNoiseGenerator::~ValNoiseGenerator()
{
}

double RT::Noise::ValNoiseGenerator::GetValue(double x, double y)
{
	x = fmod(x, 1.0);
	y = fmod(y, 1.0);

	x = (x + 1) / 2.0;
	y = (y + 1) / 2.0;

	double gridSpacing = 1.0 / static_cast<double>(m_scale);

	double localX = fmod(x, gridSpacing);
	double localY = fmod(y, gridSpacing);

	int minX = static_cast<int>((x - localX) * m_scale);
	int minY = static_cast<int>((y - localY) * m_scale);

	int c1Xi = std::max(minX, 0);
	int c1Yi = std::max(minY, 0);
	int c2Xi = std::min(minX + 1, m_scale);
	int c2Yi = std::max(minY, 0);

	int c3Xi = std::max(minX, 0);
	int c3Yi = std::min(minY + 1, m_scale);
	int c4Xi = std::min(minX + 1, m_scale);
	int c4Yi = std::min(minY + 1, m_scale);

	double v1 = m_valueGrid[c1Xi][c1Yi];
	double v2 = m_valueGrid[c2Xi][c2Yi];
	double v3 = m_valueGrid[c3Xi][c3Yi];
	double v4 = m_valueGrid[c4Xi][c4Yi];

	double xWeight = localX * static_cast<double>(m_scale);
	double yWeight = localY * static_cast<double>(m_scale);

	double t1 = Lerp(v1, v3, yWeight);
	double t2 = Lerp(v2, v4, yWeight);

	return Lerp(t1, t2, xWeight);
}

void RT::Noise::ValNoiseGenerator::SetupGrid(int scale)
{
	m_scale = scale;

	std::random_device randDev;
	std::seed_seq seed{randDev(), randDev(), randDev(), randDev() , randDev()};
	
	std::mt19937 randGen(seed);

	std::uniform_real_distribution<double> randomDist(0.0, 1.0);

	m_valueGrid.clear();

	m_valueGrid.resize(m_scale + 1, std::vector<double>(m_scale + 1, 0.0));

	for (int x = 0; x <= m_scale; ++x) {
		for (int y = 0; y <= m_scale; ++y) {
			m_valueGrid[x][y] = randomDist(randGen);
		}
	}

	if (m_wrap) {
		for (int x = 0; x <= m_scale; ++x) {
			m_valueGrid[x][m_scale] = m_valueGrid[x][0];
		}
		for (int y = 0; y <= m_scale; ++y) {
			m_valueGrid[m_scale][y] = m_valueGrid[0][y];
		}
	}

}
