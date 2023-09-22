#include "n_image.h"

RT::Normal::Image::Image() {

}

RT::Normal::Image::~Image() {

}

bool RT::Normal::Image::LoadImage(std::string fileName)
{
	if (m_imageLoaded)
		SDL_FreeSurface(m_imageSurface);

	m_fileName = fileName;
	m_imageSurface = SDL_LoadBMP(fileName.c_str());

	if (!m_imageSurface)
	{
		std::cout << "Failed to load image. " << SDL_GetError() << "." << std::endl;
		m_imageLoaded = false;
		return false;
	}

	m_xSize = m_imageSurface->w;
	m_ySize = m_imageSurface->h;
	m_pitch = m_imageSurface->pitch;
	m_pixelFormat = m_imageSurface->format;
	m_bytesPerPixel = m_pixelFormat->BytesPerPixel;

	m_imageLoaded = true;
	return true;
}

glm::dvec3 RT::Normal::Image::ComputePerturbation(const glm::dvec3& normal, const glm::dvec2& uvCoords)
{
	double xD = 0.0;
	double yD = 0.0;
	double zD = 0.0;

	if (m_imageLoaded) {
		glm::dvec2 inputLoc = uvCoords;
		glm::dvec2 newLoc = ApplyTransform(inputLoc);

		double u = newLoc[0];
		double v = newLoc[1];

		u = fmod(u, 1.0);
		v = fmod(v, 1.0);

		// Convert (u,v) to image dimensions (x,y).
		double xsd = static_cast<double>(m_xSize);
		double ysd = static_cast<double>(m_ySize);
		double xF = ((u + 1.0) / 2.0) * xsd;
		double yF = ysd - (((v + 1.0) / 2.0) * ysd);
		int x = static_cast<int>(round(xF));
		int y = static_cast<int>(round(yF));
		int xMin = static_cast<int>(floor(xF));
		int yMin = static_cast<int>(floor(yF));
		int xMax = static_cast<int>(ceil(xF));
		int yMax = static_cast<int>(ceil(yF));

		// Perform bilinear interpolation.
		double r0, g0, b0, a0;
		double r1, g1, b1, a1;
		double r2, g2, b2, a2;
		double r3, g3, b3, a3;
		GetPixelValue(xMin, yMin, r0, g0, b0, a0);
		GetPixelValue(xMax, yMin, r1, g1, b1, a1);
		GetPixelValue(xMin, yMax, r2, g2, b2, a2);
		GetPixelValue(xMax, yMax, r3, g3, b3, a3);
		double interpR = BilinearInterp(xMin, yMin, r0, xMax, yMin, r1, xMin, yMax, r2, xMax, yMax, r3, xF, yF);
		double interpG = BilinearInterp(xMin, yMin, g0, xMax, yMin, g1, xMin, yMax, g2, xMax, yMax, g3, xF, yF);
		double interpB = BilinearInterp(xMin, yMin, b0, xMax, yMin, b1, xMin, yMax, b2, xMax, yMax, b3, xF, yF);

		// Use the RGB values (ignore alpha) for the perturbation.
		xD = interpR;
		yD = interpG;
		zD = interpB;
	}

	if (m_reverseXY) {
		xD = -xD;
		yD = -yD;
	}
	
	glm::dvec3 perturbation(xD, yD, zD);

	return PerturbNormal(normal, perturbation);
}

double RT::Normal::Image::LinearInterp(const double& x0, const double& y0, const double& x1, const double& y1, const double x)
{
	double output;

	if ((x1 - x0) == 0.0)
		output = y0;
	else
		output = y0 + ((x - x0) * ((y1 - y0) / (x1 - x0)));

	return output;
}

double RT::Normal::Image::BilinearInterp(const double& x0, const double& y0, const double& v0, const double& x1, const double& y1, const double& v1, const double& x2, const double& y2, const double& v2, const double& x3, const double& y3, const double& v3, const double& x, const double& y)
{
	double p1 = LinearInterp(x0, v0, x1, v1, x);
	double p2 = LinearInterp(x2, v2, x3, v3, x);
	double p3 = LinearInterp(y0, p1, y2, p2, y);
	return p3;
}

void RT::Normal::Image::GetPixelValue(int x, int y, double& red, double& green, double& blue, double& alpha)
{
	if ((x >= 0) && (x < m_xSize) && (y >= 0) && (y < m_ySize))
	{
		// Convert (x,y) to a linear index, in bytes.
		int pixelIndex = (x * m_bytesPerPixel) + (y * m_pitch);

		// Get a pointer to the pixel data.
		uint8_t* pixels = (uint8_t*)m_imageSurface->pixels;

		// Extract the current pixel value.
		std::array<uint8_t, 4> byteValues;
		uint32_t currentPixel = 0;
		for (int i = 0; i < m_bytesPerPixel; ++i)
		{
			byteValues[i] = pixels[pixelIndex + i];
		}

		// Assemble the final pixel value, according to the number of bytes per pixel.
		switch (m_bytesPerPixel)
		{
		case 3:
			currentPixel = byteValues[0] | (byteValues[1] << 8) | (byteValues[2] << 16);
			break;

		case 4:
			currentPixel = byteValues[0] | (byteValues[1] << 8) | (byteValues[2] << 16) | (byteValues[3] << 24);
			break;
		}

		// Convert to RGB.
		uint8_t r, g, b, a;
		SDL_GetRGBA(currentPixel, m_imageSurface->format, &r, &g, &b, &a);

		// Return the color.		
		red = static_cast<double>(r - 128) / 128.0;
		green = static_cast<double>(g - 128) / 128.0;
		blue = static_cast<double>(b) / 255.0;
	}
}

//void RT::Normal::Image::GetPixelValue(int x, int y, double& red, double& green, double& blue, double& alpha)
//{
//	if ((x >= 0) && (x < m_xSize) && (y >= 0) && (y < m_ySize))
//	{
//		uint32_t currentPixel = get_pixel(m_imageSurface, x, y);
//
//		// Convert to RGB.
//		uint8_t r = 0, g = 0, b = 0, a = 0;
//
//		SDL_GetRGBA(currentPixel, m_imageSurface->format, &r, &g, &b, &a);
//
//		red = static_cast<double>(r - 128) / 128.0;
//		green = static_cast<double>(g - 128) / 128.0;
//		blue = static_cast<double>(b) / 255;
//	}
//}