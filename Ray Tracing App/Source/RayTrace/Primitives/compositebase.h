#pragma once

#include "objectbase.h"
#include "box.h"

namespace RT {
	namespace SHAPES {
		class CompositeBase : public ObjectBase {
		public:
			CompositeBase();
			virtual ~CompositeBase() override;

			void AddSubShape(std::shared_ptr<RT::ObjectBase> subShape);

			virtual void GetExtents(glm::dvec2& xLim, glm::dvec2& yLim, glm::dvec2& zLim) override;

			virtual bool TestIntersection(const Ray& castRay, RT::DATA::hitData& hitData) override;

			void UpdateBounds();

		private:
			int TestIntersection(const Ray& castRay, const Ray& bckRay, glm::dvec3& intPoint, double& currentDist, DATA::hitData& hitData);

		public:
			RT::Box m_boundingBox = RT::Box();
			
			bool m_useBoundingBox = true;

			std::vector<std::shared_ptr<ObjectBase>> m_shapeList;

			glm::dvec2 m_xLim = glm::dvec2(1e6, -1e6);
			glm::dvec2 m_yLim = glm::dvec2(1e6, -1e6);
			glm::dvec2 m_zLim = glm::dvec2(1e6, -1e6);
		};
	}

}