#pragma once

#include "normalbase.h"

namespace RT {
	namespace Normal {
		class TextureNormal : public NormalBase {
		public:
			TextureNormal();
			virtual ~TextureNormal();

			virtual glm::dvec3 ComputePerturbation(const glm::dvec3& normal, const glm::dvec2& uvCoords) override;

			void AssignBaseTexture(const std::shared_ptr<RT::Texture::TextureBase>& inputTexture);

		public:
			double m_scale = 1.0;
			bool m_reverse = false;

		private:
			bool m_haveTexture = false;
			std::shared_ptr<Texture::TextureBase> m_p_baseTexture;
		};
	}
}