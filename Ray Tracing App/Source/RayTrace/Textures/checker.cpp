#include "checker.h"
#include "flat.h"

RT::Texture::Checker::Checker()
{
	Texture::Flat color1;
	Texture::Flat color2;

	glm::dvec4 c1(1.0, 1.0, 1.0, 1.0);
	glm::dvec4 c2(0.2, 0.2, 0.2, 1.0);
	
	color1.SetColor(c1);
	color2.SetColor(c2);

	m_p_color1 = std::make_shared<Texture::Flat>(color1);
	m_p_color2 = std::make_shared<Texture::Flat>(color2);
}

RT::Texture::Checker::~Checker()
{
}

glm::dvec4 RT::Texture::Checker::GetColor(const glm::dvec2& uvCoords)
{
	glm::dvec2 inputLoc = uvCoords;
	glm::dvec2 newLoc = ApplyTransform(inputLoc);

	double newU = newLoc[0];
	double newV = newLoc[1];

	glm::dvec4 localColor(0.0);

	int check = static_cast<int>(floor(newU) + static_cast<int>(floor(newV)));

	if (check % 2 == 0) {
		localColor = m_p_color1->GetColor(uvCoords);
	}
	else {
		localColor = m_p_color2->GetColor(uvCoords);
	}

	return localColor;
}

void RT::Texture::Checker::SetColor(const glm::dvec4& inputColor1, const glm::dvec4& inputColor2)
{
	auto color1 = std::make_shared<Texture::Flat>(Flat());
	auto color2 = std::make_shared<Texture::Flat>(Flat());

	color1->SetColor(inputColor1);
	color2->SetColor(inputColor2);

	m_p_color1 = color1;
	m_p_color2 = color2;
}

void RT::Texture::Checker::SetColor(const std::shared_ptr<Texture::TextureBase>& inputColor1, const std::shared_ptr<Texture::TextureBase>& inputColor2)
{
	m_p_color1 = inputColor1;
	m_p_color2 = inputColor2;
}
