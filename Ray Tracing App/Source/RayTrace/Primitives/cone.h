#pragma once

#include "objectbase.h"
#include "../geotransform.h"

namespace RT {
	class Cone : public ObjectBase, public std::enable_shared_from_this<RT::Cone> {
	public: 
		Cone();

		virtual ~Cone();

		virtual bool TestIntersection(const Ray& castRay, DATA::hitData& hitData) override;
	};
}