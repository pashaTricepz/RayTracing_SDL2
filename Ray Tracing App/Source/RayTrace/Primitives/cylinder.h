#pragma once

#include "objectbase.h"
#include "../geotransform.h"

namespace RT {
	class Cylinder : public ObjectBase, public std::enable_shared_from_this<RT::Cylinder> {
	public:
		Cylinder();

		virtual ~Cylinder() override;

		virtual bool TestIntersection(const Ray& castRay, DATA::hitData& hitData) override;
	};
}