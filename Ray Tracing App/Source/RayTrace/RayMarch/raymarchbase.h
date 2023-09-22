#pragma once

#include <functional>

#include "sdf.h"

#include "../Primitives/objectbase.h"
#include "../Primitives/box.h"

namespace RT {
	namespace RM {
		class RayMarchBase : public ObjectBase, public std::enable_shared_from_this<RM::RayMarchBase> {
		public:
			RayMarchBase();

			virtual ~RayMarchBase() override;

			virtual bool TestIntersection(const Ray& castRay, DATA::hitData& hitData) override;

			void SetObjectFcn(std::function<double(glm::dvec3*, glm::dvec3*)> objectFcn);

			double EvaluateSDF(glm::dvec3* location, glm::dvec3* parms);

		public:
			RT::Box m_boundingBox = RT::Box();

			glm::dvec3 m_parms = { 0.0,0.0,0.0 };

		private:
			std::function<double(glm::dvec3*, glm::dvec3*)> m_objectFcn;

			bool m_haveObjectFcn = false;
			double m_epsilon = 0.0;
			int m_maxSteps = 0;

			const double m_h = 0.001;

			glm::dvec3 m_xDisp = { m_h, 0.0, 0.0 };
			glm::dvec3 m_yDisp = { 0.0, m_h, 0.0 };
			glm::dvec3 m_zDisp = { 0.0, 0.0, m_h };
		};
	}
}