#include "flat.h"

RT::Texture::Flat::Flat()
{
	m_color = { 1.0, 0.0, 0.0, 1.0 };
}

RT::Texture::Flat::~Flat()
{

}

glm::dvec4 RT::Texture::Flat::GetColor(const glm::dvec2& uv_Coords)
{
	return m_color;
}

void RT::Texture::Flat::SetColor(const glm::dvec4& inputColor)
{
	m_color = inputColor;
}
