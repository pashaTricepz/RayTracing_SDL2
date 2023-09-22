#pragma once

#include "noisebase.h"
#include <glm/gtc/constants.hpp>

namespace RT {
	namespace Noise {
		class GrdNoiseGenerator : public NoiseBase {
		public:
			GrdNoiseGenerator();

			virtual ~GrdNoiseGenerator() override;

			virtual double GetValue(double x, double y) override;

			virtual void SetupGrid(int scale) override;

		private:
			std::vector<double> ComputeNormDisp(double x1, double y1, double x2, double y2);

		public:
			std::vector<std::vector<double>> m_vectorGridX;
			std::vector<std::vector<double>> m_vectorGridY;

			bool m_wrap = false;
		};
	}

}