#include "grdnoisegenerator.h"

RT::Noise::GrdNoiseGenerator::GrdNoiseGenerator()
{
	SetupGrid(2);
}

RT::Noise::GrdNoiseGenerator::~GrdNoiseGenerator()
{
}

double RT::Noise::GrdNoiseGenerator::GetValue(double x, double y)
{
	x = fmod(x, 1.0);
	y = fmod(y, 1.0);

	x = (x + 1.0) / 2.0;
	y = (y + 1.0) / 2.0;

	double gridSpacing = 1.0 / static_cast<double>(m_scale);

	double localX = fmod(x, gridSpacing);
	double localY = fmod(y, gridSpacing);

	int minX = static_cast<int>((x - localX) * m_scale);
	int minY = static_cast<int>((y - localY) * m_scale);

	int C1Xi = std::max(minX, 0);
	int C1Yi = std::max(minY, 0);
	int C2Xi = std::min(minX + 1, m_scale);
	int C2Yi = std::max(minY, 0);

	int C3Xi = std::max(minX, 0);
	int C3Yi = std::min(minY + 1, m_scale);
	int C4Xi = std::min(minX + 1, m_scale);
	int C4Yi = std::min(minY + 1, m_scale);


	std::vector<double> v1{m_vectorGridX[C1Xi][C1Yi], m_vectorGridY[C1Xi][C1Yi]};
	std::vector<double> v2{m_vectorGridX[C2Xi][C2Yi], m_vectorGridY[C2Xi][C2Yi]};
	std::vector<double> v3{m_vectorGridX[C3Xi][C3Yi], m_vectorGridY[C3Xi][C3Yi]};
	std::vector<double> v4{m_vectorGridX[C4Xi][C4Yi], m_vectorGridY[C4Xi][C4Yi]};


	double c1X = static_cast<double>(C1Xi) * gridSpacing;
	double c1Y = static_cast<double>(C1Yi) * gridSpacing;
	double c2X = static_cast<double>(C2Xi) * gridSpacing;
	double c2Y = static_cast<double>(C2Yi) * gridSpacing;
	double c3X = static_cast<double>(C3Xi) * gridSpacing;
	double c3Y = static_cast<double>(C3Yi) * gridSpacing;
	double c4X = static_cast<double>(C4Xi) * gridSpacing;
	double c4Y = static_cast<double>(C4Yi) * gridSpacing;

	std::vector<double> d1 = ComputeNormDisp(x, y, c1X, c1Y);
	std::vector<double> d2 = ComputeNormDisp(x, y, c2X, c2Y);
	std::vector<double> d3 = ComputeNormDisp(x, y, c3X, c3Y);
	std::vector<double> d4 = ComputeNormDisp(x, y, c4X, c4Y);

	double dp1 = v1[0] * d1[0] + v1[1] * d1[1];
	double dp2 = v2[0] * d2[0] + v2[1] * d2[1];
	double dp3 = v3[0] * d3[0] + v3[1] * d3[1];
	double dp4 = v4[0] * d4[0] + v4[1] * d4[1];

	double xWeight = localX * static_cast<double>(m_scale);
	double yWeight = localY * static_cast<double>(m_scale);

	double t1 = Lerp(dp1, dp3, yWeight);
	double t2 = Lerp(dp2, dp4, yWeight);

	return Lerp(t1, t2, xWeight);
}

void RT::Noise::GrdNoiseGenerator::SetupGrid(int scale)
{
	m_scale = scale;

	std::random_device randDev;
	std::seed_seq seed{randDev(), randDev(), randDev(), randDev()};

	std::mt19937 randGen;

	std::uniform_real_distribution<double> randomDist(0.0, 1.0);

	m_vectorGridX.clear();
	m_vectorGridY.clear();

	m_vectorGridX.resize(m_scale + 1, std::vector<double>(m_scale + 1, 0.0));
	m_vectorGridY.resize(m_scale + 1, std::vector<double>(m_scale + 1, 0.0));

	for (int x = 0; x <= m_scale; ++x) {
		for (int y = 0; y <= m_scale; ++y) {
			double theta = randomDist(randGen) * 2.0 * glm::pi<double>();

			m_vectorGridX[x][y] = cos(theta);
			m_vectorGridY[x][y] = sin(theta);
		}
	}


	if (m_wrap) {
		for (int i = 0; i <= m_scale; ++i) {
			m_vectorGridX[i][m_scale] = m_vectorGridX[i][0];
			m_vectorGridY[i][m_scale] = m_vectorGridY[i][0];

			m_vectorGridX[m_scale][i] = m_vectorGridX[0][i];
			m_vectorGridY[m_scale][i] = m_vectorGridY[0][i];
		}
	}


}

std::vector<double> RT::Noise::GrdNoiseGenerator::ComputeNormDisp(double x1, double y1, double x2, double y2)
{
	return std::vector<double>{x1 - x2, y1 - y2};
}
