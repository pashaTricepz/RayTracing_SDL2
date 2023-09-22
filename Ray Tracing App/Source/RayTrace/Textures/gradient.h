#pragma once

#include "texturebase.h"
#include "colormap.h"

namespace RT {
	namespace Texture {
		class Gradient : public TextureBase {
		public:
			Gradient();
			virtual ~Gradient() override;

			virtual glm::dvec4 GetColor(const glm::dvec2& uvCoords) override;

			virtual double GetValue(const glm::dvec2& uvCoords) override;

			void SetStop(double position, const glm::dvec4& value);

		private:
			Texture::ColorMap m_colorMap;
		};
	}
}