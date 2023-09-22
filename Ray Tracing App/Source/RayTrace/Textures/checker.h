#pragma once

#include "texturebase.h"

#include <memory>

namespace RT {
	namespace Texture {
		class Checker : public TextureBase {
		public:
			Checker();
			virtual ~Checker();

			virtual glm::dvec4 GetColor(const glm::dvec2& uvCoords) override;

			void SetColor(const glm::dvec4& inputColor1, const glm::dvec4& inputColor2);
			void SetColor(const std::shared_ptr<Texture::TextureBase>& inputColor1, const std::shared_ptr<Texture::TextureBase>& inputColor2);

		private:
			std::shared_ptr<Texture::TextureBase> m_p_color1;
			std::shared_ptr<Texture::TextureBase> m_p_color2;
		};

	}
}