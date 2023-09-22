#include "noisebase.h"

RT::Noise::NoiseBase::NoiseBase()
{
	m_scale = 1;
}

RT::Noise::NoiseBase::~NoiseBase()
{
}

double RT::Noise::NoiseBase::GetValue(double u, double v)
{
	return 0.0;
}

double RT::Noise::NoiseBase::Lerp(double v1, double v2, double iPos)
{
	double fade = iPos * iPos * (3 - 2 * iPos);
	return v1 + fade * (v2 - v1);
}

void RT::Noise::NoiseBase::SetupGrid(int scale)
{
	m_scale = scale;
}
