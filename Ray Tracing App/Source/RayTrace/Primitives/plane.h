#pragma once

#include "objectbase.h"
#include "../geotransform.h"


namespace RT {
	class Plane : public ObjectBase, public std::enable_shared_from_this<RT::Plane> {

	public:
		Plane();

		virtual ~Plane();

		virtual bool TestIntersection(const Ray& castRay, DATA::hitData& hitData) override;
	};

}
