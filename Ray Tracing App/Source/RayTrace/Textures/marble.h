#pragma once

#include "../Noise/grdnoisegenerator.h"

#include "texturebase.h"
#include "colormap.h"

namespace RT {
	namespace Texture {
		class Marble : public TextureBase {
		public:
			Marble();
			virtual ~Marble() override;

			virtual glm::dvec4 GetColor(const glm::dvec2& uvCoords) override;

			virtual double GetValue(const glm::dvec2& uvCoords) override;

			void SetColorMap(const std::shared_ptr<Texture::ColorMap>& colorMap);

			void SetAmplitude(double amplitude1, double amplitude2);

			void SetScale(int scale1, int scale2);

			void SetMinMax(double minValue, double maxValue);

			void SetSine(double amplitude, double frequency);

		public:
			std::shared_ptr<Texture::ColorMap> m_colorMap;
			bool m_haveColorMap = false;

			std::vector<Noise::GrdNoiseGenerator> m_noiseGeneratorList;

			double m_amplitude1 = 8.0;
			double m_amplitude2 = 8.0;

			int m_scale1 = 4;
			int m_scale2 = 40;

			double m_maxValue = 1.0;
			double m_minValue = -1.0;

			double m_sineAmplitude = 0.25;
			double m_sineFrequency = 4.0;
		};
	}
}