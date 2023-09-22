#include "texturebase.h"

RT::Texture::TextureBase::TextureBase()
{
}

RT::Texture::TextureBase::~TextureBase()
{
}

glm::dvec4 RT::Texture::TextureBase::GetColor(const glm::dvec2& uvCoords)
{
	glm::dvec4 outputColor(0.0);
	return outputColor;
}

double RT::Texture::TextureBase::GetValue(const glm::dvec2& uvCoords)
{
	return 0.0;
}

void RT::Texture::TextureBase::SetTransform(const glm::dvec2& translation, const double& rotation, const glm::dvec2& scale)
{
	glm::dmat3x3 rotationMatrix(cos(rotation), -sin(rotation), 0.0,
								sin(rotation),  cos(rotation), 0.0, 
										  0.0,			  0.0, 1.0);

	glm::dmat3x3 scaleMatrix(scale[0],		0.0, 0.0,
								  0.0, scale[1], 0.0,
								  0.0,		0.0, 1.0);

	glm::dmat3x3 translationMatrix(1.0, 0.0, translation[0],
								   0.0, 1.0, translation[1],
								   0.0, 0.0,		    1.0);

	m_transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;
}

glm::dvec3 RT::Texture::TextureBase::BlendColors(const std::vector<glm::dvec4>& inputColorList)
{
	return glm::dvec3(0.0);
}

glm::dvec2 RT::Texture::TextureBase::ApplyTransform(const glm::dvec2& inputVector)
{
	glm::dvec3 newInput(0.0);

	newInput[0] = inputVector[0];
	newInput[1] = inputVector[1];

	glm::dvec3 result = newInput * m_transformMatrix;

	glm::dvec2 output(result[0], result[1]);

	return output;
}
