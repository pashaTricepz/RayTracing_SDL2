#pragma once

#include <vector>
#include <random>

namespace RT {
	namespace Noise {
		class NoiseBase {
		public:
			NoiseBase();
			virtual ~NoiseBase();

			virtual double GetValue(double u, double v);

			double Lerp(double v1, double v2, double iPos);

			virtual void SetupGrid(int scale);

		public:
			int m_scale;
		};
	}
}
