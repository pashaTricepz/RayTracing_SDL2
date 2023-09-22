#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

namespace RT {
	class Ray {
	public:
		Ray();
		Ray(const glm::dvec3& point1, glm::dvec3& point2);

		glm::dvec3 GetPoint1() const;
		glm::dvec3 GetPoint2() const;

	public:
		glm::dvec3 m_point1;
		glm::dvec3 m_point2;

		glm::dvec3 m_lab;
	};
}