#pragma once

#include "ray.h"

#include <iostream>
#include <iomanip>

namespace RT {

	constexpr bool FWDTFORM = true;
	constexpr bool BCKTFORM = false;

	class GTform {
	public:
		GTform();
		GTform(const glm::dvec3& translation, const glm::dvec3& rotation, const glm::dvec3& scale);
		~GTform();

		GTform(const glm::dmat4x4& fwd, const glm::dmat4x4& bck);

		void SetTransform(const glm::dvec3& translation, const glm::dvec3& rotation, const glm::dvec3& scale);

		glm::dmat4x4 GetForward();
		glm::dmat4x4 GetBackward();

		Ray			Apply(const Ray& inputRay, bool dirFlag);
		glm::dvec3	Apply(const glm::dvec3& inputVector, bool dirFlag);
		glm::dvec3	ApplyNorm(const glm::dvec3& inputVector);
		
		glm::dmat3x3 GetNormalTransform();

		friend GTform operator*(const GTform& lhs, const GTform& rhs);

		GTform operator=(const GTform& rhs);

		void PrintMatrix(bool dirFlag);

	private:
		void Print(const glm::dmat4x4& matrix);
		void Print(const glm::dmat3x3& matrix);
		void ExtractLinearTransform();

	private:
		glm::dmat4x4 m_fwdtfm;
		glm::dmat4x4 m_bcktfm;
		glm::dmat3x3 m_lintfm;
	};	
}