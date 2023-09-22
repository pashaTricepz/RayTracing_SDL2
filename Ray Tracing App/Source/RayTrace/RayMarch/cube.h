#pragma once

#include "raymarchbase.h"

namespace RT
{
	namespace RM
	{
		class Cube : public RayMarchBase
		{
		public:
			// Constructor.
			Cube();

			// Destructor.
			virtual ~Cube() override;

		private:
			// Private object function.
			double ObjectFcn(glm::dvec3* location, glm::dvec3* parms);
		};
	}
}