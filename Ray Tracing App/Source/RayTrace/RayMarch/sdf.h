#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

namespace RT {
	namespace RM {
		namespace SDF {
			double Sphere(const glm::dvec3& p, const glm::dvec3& center, const glm::dvec3& params);
			double Torus(const glm::dvec3& p, const glm::dvec3& center, const glm::dvec3& params);
			double Box(const glm::dvec3& p, const glm::dvec3& center, const glm::dvec3& params);
		}
	}
}