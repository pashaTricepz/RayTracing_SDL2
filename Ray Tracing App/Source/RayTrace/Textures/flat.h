#pragma once

#include "texturebase.h"

namespace RT {
	namespace Texture {
		class Flat : public TextureBase {
		public:
			Flat();
			virtual ~Flat() override;

			virtual glm::dvec4 GetColor(const glm::dvec2& uv_Coords) override;

			void SetColor(const glm::dvec4& inputColor);

		private:
			glm::dvec4 m_color;
		};
	}
}