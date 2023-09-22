#pragma once

#include "../Textures/texturebase.h"

#include <memory>

namespace RT {
	namespace Normal {
		class NormalBase {
		public:
			NormalBase();
			virtual ~NormalBase();

			virtual glm::dvec3 ComputePerturbation(const glm::dvec3& normal, const glm::dvec2& uvCoords);

			glm::dvec3 PerturbNormal(const glm::dvec3& normal, glm::dvec3& perturbation);
			
			glm::dvec2 TextureDiff(const std::shared_ptr<Texture::TextureBase>& inputeTexture, const glm::dvec2& uvCoords);

			void SetAmplitude(double amplitude);

			void SetTransform(const glm::dvec2& translation, const double& rotation, const glm::dvec2& scale);

			glm::dvec2 ApplyTransform(const glm::dvec2& inputVector);

		public:
			double m_amplitude = 1.0;

		private:
			glm::dmat3x3 m_transformMatrix;
		};
	}
}