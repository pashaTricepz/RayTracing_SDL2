#pragma once

#include "normalbase.h"

namespace RT {
	namespace Normal {
		class Constant : public NormalBase {
		public:
			Constant();
			virtual ~Constant();

			virtual glm::dvec3 ComputePerturbation(const glm::dvec3& normal, const glm::dvec2& uvCoords) override;

		public:
			glm::dvec3 m_displacement;
		};
	}
}