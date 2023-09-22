#pragma once
#pragma once

#include "texturebase.h"

#include <string>
#include <iostream>
#include <array>

namespace RT {
	namespace Texture {
		class Image : public TextureBase {
		public:
			Image();
			virtual ~Image() override;

			// Function to return the color.
			virtual glm::dvec4 GetColor(const glm::dvec2& uvCoords) override;

			// Function to load the image to be used.
			bool LoadImage(std::string fileName);

		private:
			double LinearInterp(const double& x0, const double& y0, const double& x1, const double& y1, const double x);
			double BilinearInterp(const double& x0, const double& y0, const double& v0,
				const double& x1, const double& y1, const double& v1,
				const double& x2, const double& y2, const double& v2,
				const double& x3, const double& y3, const double& v3,
				const double& x, const double& y);

			void GetPixelValue(int x, int y, double& red, double& green, double& blue, double& alpha);

		private:
			std::string m_fileName = "";
			SDL_Surface* m_imageSurface;
			SDL_PixelFormat* m_pixelFormat;
			bool m_imageLoaded = false;
			int m_xSize = 128, m_ySize = 128, m_pitch = 384;
			uint8_t m_bytesPerPixel = 3;
			uint32_t m_rMask = 0;
			uint32_t m_gMask = 0;
			uint32_t m_bMask = 0;
			uint32_t m_aMask = 0;

		};
	}
}
