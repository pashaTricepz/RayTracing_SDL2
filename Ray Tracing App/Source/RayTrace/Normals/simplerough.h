#pragma once

#include "normalbase.h"

#include <random>

namespace RT {
	namespace Normal {
		class SimpleRough : public NormalBase {
		public:
			SimpleRough();
			virtual ~SimpleRough();

			virtual glm::dvec3 ComputePerturbation(const glm::dvec3& normal, const glm::dvec2& uvCoords) override;

		private:
			std::shared_ptr<std::mt19937> m_p_randGen;
		};
	}
}