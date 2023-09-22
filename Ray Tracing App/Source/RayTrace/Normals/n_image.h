#pragma once

#include "normalbase.h"

#include <SDL.h>

#include <random>
#include <iostream>
#include <array>

namespace RT {
	namespace Normal {
		class Image : public NormalBase {
		public:
			Image();
			virtual ~Image() override;

			bool LoadImage(std::string fileName);

			virtual glm::dvec3 ComputePerturbation(const glm::dvec3& normal, const glm::dvec2& uvCoords) override;

		private:
			double LinearInterp(const double& x0, const double& y0, const double& x1, const double& y1, const double x);
			double BilinearInterp(const double& x0, const double& y0, const double& v0,
								  const double& x1, const double& y1, const double& v1,
								  const double& x2, const double& y2, const double& v2,
								  const double& x3, const double& y3, const double& v3,
								  const double& x, const double& y);

			void GetPixelValue(int x, int y, double& red, double& green, double& blue, double& alpha);

		public:
			bool m_reverseXY = false;

		private:
			glm::dmat3x3 m_transformMatrix = glm::dmat3x3(1.0);

			std::string m_fileName;
			SDL_Surface* m_imageSurface;
			SDL_PixelFormat* m_pixelFormat;
			bool m_imageLoaded = false;
			int m_xSize, m_ySize, m_pitch;
			uint8_t m_bytesPerPixel;
		};
	}
}

