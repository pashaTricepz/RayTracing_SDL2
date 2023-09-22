#include "texturenormal.h"

RT::Normal::TextureNormal::TextureNormal()
{

}

RT::Normal::TextureNormal::~TextureNormal()
{
}

glm::dvec3 RT::Normal::TextureNormal::ComputePerturbation(const glm::dvec3& normal, const glm::dvec2& uvCoords)
{
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	if (m_haveTexture) {
		glm::dvec2 uvGrad = TextureDiff(m_p_baseTexture, uvCoords);
		if (!m_reverse) {
			x = -uvGrad[0] * m_scale;
			y = -uvGrad[1] * m_scale;
		}
		else {
			x = uvGrad[0] * m_scale;
			y = uvGrad[1] * m_scale;
		}
	}
	glm::dvec3 perturbation(x, y, z);
	return PerturbNormal(normal, perturbation);
}

void RT::Normal::TextureNormal::AssignBaseTexture(const std::shared_ptr<RT::Texture::TextureBase>& inputTexture)
{
	m_p_baseTexture = inputTexture;
	m_haveTexture = true;
}
