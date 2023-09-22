#pragma once

#include "../Normals/normalbase.h"
#include "../Textures/texturebase.h"
#include "../Primitives/objectbase.h"
#include "../Lights/lightbase.h"
#include "../ray.h"
#include "../utils.h"
 
#include <vector>

namespace RT {
	class MaterialBase {
	public:
		MaterialBase();
		virtual ~MaterialBase();


		virtual glm::dvec3 ComputeColor(const std::vector<std::shared_ptr<ObjectBase>>& objectList,
										const std::vector<std::shared_ptr<LightBase>>& lightBase,
										const std::shared_ptr<ObjectBase>& currentObject,
										const glm::dvec3& intPoint, const glm::dvec3& localNormal,
										const glm::dvec3& localPOI, const glm::dvec2& uvCoords,
										const Ray& cameraRay);

		static glm::dvec3 ComputeDiffuseColor(	const std::vector<std::shared_ptr<ObjectBase>>& objectList,
												const std::vector<std::shared_ptr<LightBase>>& lightBase,
												const std::shared_ptr<ObjectBase>& currentObject,
												const glm::dvec3& intPoint, const glm::dvec3& localNormal,
												const glm::dvec3& baseColor);

		glm::dvec3 ComputeReflectionColor(	const std::vector<std::shared_ptr<ObjectBase>>& objectList,
											const std::vector<std::shared_ptr<LightBase>>& lightBase,
											const std::shared_ptr<ObjectBase>& currentObject,
											const glm::dvec3& intPoint, const glm::dvec3& localNormal,
											const Ray& incidentRay);

		glm::dvec3 ComputeSpecAndDiffuse(const std::vector<std::shared_ptr<ObjectBase>>& objectList,
											const std::vector<std::shared_ptr<LightBase>>& lightBase,
											const std::shared_ptr<ObjectBase>& currentObject,
											const glm::dvec3& intPoint, const glm::dvec3& localNormal,
											const glm::dvec3& baseColor, const Ray& cameraRay);

		bool CastRay(	const Ray& castRay, const std::vector<std::shared_ptr<ObjectBase>>& objectList,
						const std::shared_ptr<ObjectBase>& thisObject,
						std::shared_ptr<ObjectBase>& closestObject,
						RT::DATA::hitData& closestHitData);

		void AssignTexture(const std::shared_ptr<Texture::TextureBase>& inputTexture);

		void AssignNormalMap(const std::shared_ptr<Normal::NormalBase>& inputNormalMap);

		glm::dvec4 GetTextureColor(const glm::dvec2& uvCoords);

		glm::dvec3 PerturbNormal(const glm::dvec3& normal, const glm::dvec2& uvCoords);

		void BlendColors(glm::dvec4& color1, const glm::dvec4& color2);

	public:
		inline static int m_maxReflectionRays;
		inline static int thread_local m_reflectionRayCount;

		inline static glm::dvec3 m_ambientColor = glm::dvec3(1.0);
		inline static double m_ambientIntensity = 0.2;

		std::vector<std::shared_ptr<Texture::TextureBase>> m_textureList;

		std::vector<std::shared_ptr<Normal::NormalBase>> m_normalMapList;

		bool m_hasTexture = false;
		bool m_hasNormalMap = false;

		glm::dvec3 m_localNormal;

		double m_specular = 0.0;
		double m_shininess = 0.0;
	};

}