#include "normalbase.h"

RT::Normal::NormalBase::NormalBase()
{
	m_transformMatrix = glm::dmat2x2(1.0);
}

RT::Normal::NormalBase::~NormalBase()
{

}

glm::dvec3 RT::Normal::NormalBase::ComputePerturbation(const glm::dvec3& normal, const glm::dvec2& uvCoords)
{
	return glm::dvec3(0.0);
}

glm::dvec3 RT::Normal::NormalBase::PerturbNormal(const glm::dvec3& normal, glm::dvec3& perturbation)
{
	glm::dvec3 newUpVector(0.0, 0.0, -1.0);
	if (normal[2] > 0.99 or normal[2] < -0.99)
		newUpVector = { 1.0, 0.0, 0.0 };

	glm::dvec3 pV = glm::normalize(glm::cross(newUpVector, normal));

	glm::dvec3 pU = glm::normalize(glm::cross(normal, pV));

	glm::dvec3 output = glm::normalize(normal + pU * perturbation[0] + pV * perturbation[1] + normal * perturbation[2]);

	return output;
}

glm::dvec2 RT::Normal::NormalBase::TextureDiff(const std::shared_ptr<Texture::TextureBase>& inputTexture, const glm::dvec2& uvCoords)
{
	double h = 0.001;

	glm::dvec2 uDisp = { h, 0.0 };
	glm::dvec2 vDisp = { 0.0, h };

	double uGrad = (inputTexture->GetValue(uvCoords + uDisp) - inputTexture->GetValue(uvCoords - uDisp)) / (2 * h);
	double vGrad = (inputTexture->GetValue(uvCoords + vDisp) - inputTexture->GetValue(uvCoords - vDisp)) / (2 * h);

	glm::dvec2 output = { uGrad, vGrad };
	return output;
}

void RT::Normal::NormalBase::SetAmplitude(double amplitude)
{
	m_amplitude = amplitude;
}

void RT::Normal::NormalBase::SetTransform(const glm::dvec2& translation, const double& rotation, const glm::dvec2& scale)
{
	glm::dmat3 rotationMatrix(cos(rotation), -sin(rotation), 0.0,
							  sin(rotation), cos(rotation), 0.0,
							  0.0, 0.0, 1.0);

	glm::dmat3 scaleMatrix(scale[0], 0.0, 0.0, 0.0, scale[1], 0.0, 0.0, 0.0, 1.0);

	glm::dmat3 translationMatrix(1.0, 0.0, translation[0],
								 0.0, 1.0, translation[1],
								 0.0, 0.0, 1.0);

	m_transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;
}

glm::dvec2 RT::Normal::NormalBase::ApplyTransform(const glm::dvec2& inputVector)
{
	glm::dvec3 newInput(0.0);
	newInput[0] = inputVector[0];
	newInput[1] = inputVector[1];

	glm::dvec3 result = newInput * m_transformMatrix;

	glm::dvec2 output(0.0);

	output[0] = result[0];
	output[1] = result[1];

	return output;
}
