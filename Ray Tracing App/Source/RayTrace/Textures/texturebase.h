#pragma once

#include "../ray.h"
#include <vector>

#include <SDL.h>

namespace RT {
	namespace Texture {
		class TextureBase {
		public:
			TextureBase();
			virtual ~TextureBase();

			virtual glm::dvec4 GetColor(const glm::dvec2& uvCoords);

			virtual double GetValue(const glm::dvec2& uvCoords);

			void SetTransform(const glm::dvec2& translation, const double& rotation, const glm::dvec2& scale);

			static glm::dvec3 BlendColors(const std::vector<glm::dvec4>& inputColorList);

			glm::dvec2 ApplyTransform(const glm::dvec2& inputVector);

		private:
			glm::dmat3x3 m_transformMatrix = glm::dmat3x3(1.0);
		};
	}

}


static inline Uint32 get_pixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1: return *p;
	case 2: return *(Uint16*)p;
	case 3: if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		return p[0] << 16 | p[1] << 8 | p[2];
		  else
		return p[0] | p[1] << 8 | p[2] << 16;
	case 4: return *(Uint32*)p;
	default: return 0;       /* shouldn't happen, but avoids warnings */
	}
}