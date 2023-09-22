#pragma once

#include "objectbase.h"
#include "../geotransform.h"

namespace RT {
	class Sphere : public ObjectBase, public std::enable_shared_from_this<RT::Sphere> {
	public:

		Sphere();

		virtual ~Sphere() override;

		virtual bool TestIntersection(const Ray& castRay, DATA::hitData& hitData) override;

	private:

	};
}