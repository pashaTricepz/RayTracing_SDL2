#pragma once

#include "objectbase.h"

#include "../geotransform.h"

namespace RT {
	class Box : public ObjectBase, public std::enable_shared_from_this<RT::Box> {
	public:
		Box();

		virtual ~Box() override;

		virtual bool TestIntersection(const Ray& castRay, DATA::hitData& hitData) override;

		bool TestIntersection(const Ray& castRay);

	private:

	};
}