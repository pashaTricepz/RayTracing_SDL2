#pragma once

#include "../ray.h"
#include "../geotransform.h"
#include "../utils.h"

#include <iostream>

#include <SDL.h>

#include <array>
#include <vector>


namespace RT {
	class MaterialBase;

	constexpr int uvSPHERE = 0;
	constexpr int uvPLANE = 1;
	constexpr int uvCYLINDER = 2;
	constexpr int uvBOX = 3;

	class ObjectBase {
	public:
		ObjectBase();
		virtual ~ObjectBase();

		virtual bool TestIntersection(const Ray& castRay, RT::DATA::hitData& hitData);

		virtual void GetExtents(glm::dvec2& xLim, glm::dvec2& yLim, glm::dvec2& zLim);
		virtual void GetExtents(const GTform& parentTransformMatrix, glm::dvec2& xLim, glm::dvec2& yLim, glm::dvec2& zLim);

		std::vector<glm::dvec3> ConstructCube(double xMin, double xMax,double yMin, double yMax, double zMin, double zMax);

		void SetTransformMatrix(const GTform& transformMatrix);

		RT::GTform GetTransformMatrix();

		bool CloseEnough(const double f1, const double f2);

		bool AssignMaterial(const std::shared_ptr<MaterialBase>& objectMaterial);

		void ComputeUV(const glm::dvec3& localPOI, glm::dvec2& uvCoords);

	public:
		std::string m_tag;

		glm::dvec3 m_baseColor;

		GTform m_transformMatrix;

		std::shared_ptr<MaterialBase> m_pMaterial;

		bool m_hasMaterial = false;

		bool m_isVisible = true;

		int m_uvMapType = RT::uvSPHERE;

		RT::GTform m_boundingBoxTransform;

		double m_boundingBoxPadding = 0.0;
	};
}