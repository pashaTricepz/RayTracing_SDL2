#pragma once

#include "../Noise/grdnoisegenerator.h"
#include "../Noise/valnoisegenerator.h"

#include "texturebase.h"
#include "colormap.h"

namespace RT {
	namespace Texture {
		class BasicNoise : public TextureBase {
		public:
			BasicNoise();
			virtual ~BasicNoise() override;

			virtual glm::dvec4 GetColor(const glm::dvec2& uvCoords) override;

			void SetColorMap(const std::shared_ptr<Texture::ColorMap>& colorMap);

			void SetAmplitude(double amplitude);

			void SetScale(int scale);
		public:
			std::shared_ptr<Texture::ColorMap> m_colorMap;
			bool m_haveColorMap = false;

			Noise::GrdNoiseGenerator m_noiseGenerator;

			double m_amplitude = 8.0;

			int m_scale = 3;
		};
	}
}