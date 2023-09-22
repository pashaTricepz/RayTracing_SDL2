#pragma once

#include <SDL.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>
#include <iostream>
#include <iomanip>
#include <vector>

namespace RT {
	class ObjectBase;

	namespace DATA {
		struct hitData {
			hitData() {
				poi = { 0.0, 0.0, 0.0 };
				normal = { 0.0, 0.0, 0.0 };
				color = { 0.0, 0.0, 0.0 };
				localPOI = { 0.0, 0.0, 0.0 };
				uvCoords = { 0.0, 0.0 };
			};
			glm::dvec3 poi;
			glm::dvec3 normal;
			glm::dvec3 color;
			glm::dvec3 localPOI;
			glm::dvec2 uvCoords;
			std::shared_ptr<RT::ObjectBase> hitObject = nullptr;
		};

		struct tile {
			int x;
			int y;
			int xSize;
			int ySize;
			int renderComplete = 0;
			bool textureComplete = false;
			SDL_Texture *pTexture;
			std::vector<glm::dvec3> rgbData;
		};
	}

	namespace UTILS {
		void PrintVector(const glm::dvec3& inputVector);
	}
}