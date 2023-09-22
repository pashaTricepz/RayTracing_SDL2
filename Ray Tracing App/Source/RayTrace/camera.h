#pragma once

#include "ray.h"

namespace RT {
	class Camera {
	public:
		Camera();

		void				SetPosition		(glm::dvec3&& newPosition);
		void				SetLookAt		(glm::dvec3&& newLookAt);
		void				SetUp			(glm::dvec3&& upVector);
		void				SetLength		(double newLength);
		void				SetAspect		(double newAspect);
		void				SetHorzSize		(double newSize);

		glm::dvec3			GetPosition();
		glm::dvec3			GetLookAt();
		glm::dvec3			GetUp();
		glm::dvec3			GetU();
		glm::dvec3			GetY();
		glm::dvec3			GetScreenCentre();

		double				GetLength();
		double				GetHorzSize();
		double				GetAspect();

		bool GenerateRay(double proScreenX, double proScreeenY, Ray& cameraRay);

		void UpdateCameraGeometry();

	private:
		glm::dvec3 m_cameraPosition;
		glm::dvec3 m_cameraLookAt;
		glm::dvec3 m_cameraUp;

		double m_cameraLength;
		double m_cameraHorSize;
		double m_cameraAspectRatio;

		glm::dvec3 m_alignmentVector;
		glm::dvec3 m_projectiomScreenU;
		glm::dvec3 m_projectiomScreenV;
		glm::dvec3 m_projectiomScreenCentre;
	};
}