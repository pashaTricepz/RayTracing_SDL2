#pragma once

#include "raymarchbase.h"

namespace RT {
	namespace RM {
		class Torus : public RayMarchBase {
		public:
			Torus();

			virtual ~Torus() override;

			void SetRadii(double r1, double r2);

		private:

			double ObjectFcn(glm::dvec3* locaton, glm::dvec3* parms);

			void UpdateBounds();

		private:
			double m_r1 = 1.0;
			double m_r2 = 0.25;

		};
	}
}
