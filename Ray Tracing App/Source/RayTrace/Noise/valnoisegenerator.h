#pragma once

#include "noisebase.h"

namespace RT {
	namespace Noise {
		class ValNoiseGenerator : public NoiseBase {
		public:
			ValNoiseGenerator();
			virtual ~ValNoiseGenerator();

			virtual double GetValue(double x, double y) override;

			virtual void SetupGrid(int scale) override;

		public:
			std::vector<std::vector<double>> m_valueGrid;

			bool m_wrap = false;
		};
	}
}
