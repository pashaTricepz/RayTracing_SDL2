#pragma once

#include <vector>
#include <chrono>

#include <SDL.h>

#include "../image.h"
#include "../camera.h"

#include "../Normals/simplerough.h"
#include "../Normals/texturenormal.h"
#include "../Normals/n_image.h"

#include "../Primitives/sphere.h"
#include "../Primitives/plane.h"
#include "../Primitives/cylinder.h"
#include "../Primitives/cone.h"
#include "../Primitives/box.h"
#include "../Primitives/compositebase.h"

#include "../Textures/checker.h"
#include "../Textures/t_image.h"
#include "../Textures/gradient.h"
#include "../Textures/colormap.h"
#include "../Textures/basicvaluenoise.h"
#include "../Textures/marble.h"
#include "../Textures/basicnoise.h"

#include "../Textures/stone1.h"

#include "../Lights/pointlight.h"

#include "../Materials/simplematerial.h"
#include "../Materials/simplerefractive.h"

#include "../RayMarch/sphere.h"
#include "../RayMarch/torus.h"
#include "../RayMarch/cube.h"

namespace RT {
	class Scene {
	public:
		Scene();
		virtual ~Scene();

		bool Render(Image& outpotImage);

		void RenderTile(RT::DATA::tile* tile);

		bool CastRay(Ray& castRay, std::shared_ptr<ObjectBase>& closestObject, DATA::hitData& closestHitData);

		virtual void SetupSceneObjects();

	private:
		glm::dvec3 RenderPixel(int x, int y, int xSize, int ySize);

		int Sub2Ind(int x, int y, int xSize, int ySize);

	public:
		RT::Camera m_camera;

		std::vector<std::shared_ptr<RT::ObjectBase>> m_objectList;
		std::vector<std::shared_ptr<RT::LightBase>> m_lightList;

		int m_xSize, m_ySize;
	};
}