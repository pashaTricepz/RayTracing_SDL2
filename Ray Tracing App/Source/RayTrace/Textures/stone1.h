#pragma once

#include "../Noise/valnoisegenerator.h"

#include "texturebase.h"
#include "colormap.h"

namespace RT {
	namespace Texture {
		class Stone1 : public TextureBase {
		public:
			Stone1();
			virtual ~Stone1() override;

			virtual glm::dvec4 GetColor(const glm::dvec2& uvCoords) override;

			virtual double GetValue(const glm::dvec2& uvCoords) override;

			void SetColorMap(const std::shared_ptr<Texture::ColorMap>& colorMap);

			void SetAmplitude(double amplitude);

			void SetScale(int scale);
		public:
			std::shared_ptr<Texture::ColorMap> m_colorMap;
			bool m_haveColorMap = false;

			Noise::ValNoiseGenerator m_noiseGenerator1;
			Noise::ValNoiseGenerator m_noiseGenerator2;

			double m_amplitude1 = 8.0;
			double m_amplitude2 = 4.0;

			int m_scale1 = 6;
			int m_scale2 = 30;

			double m_minValue, m_maxValue;
		};
	}
}