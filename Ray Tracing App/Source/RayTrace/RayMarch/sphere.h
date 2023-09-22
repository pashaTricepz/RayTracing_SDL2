#pragma once

#include "raymarchbase.h"

namespace RT {
	namespace RM {
		class Sphere : public RayMarchBase {
		public:
			Sphere();

			virtual ~Sphere() override;

		private:
			double ObjectFcn(glm::dvec3* location, glm::dvec3* parms);
		};
	}
}