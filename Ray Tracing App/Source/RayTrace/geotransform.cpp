#include "geotransform.h"

RT::GTform::GTform()
{
    m_fwdtfm = glm::dmat4x4(1.0);
    m_bcktfm = glm::dmat4x4(1.0);
    m_lintfm = glm::dmat3x3(1.0);
    ExtractLinearTransform();
}

RT::GTform::GTform(const glm::dvec3& translation, const glm::dvec3& rotation, const glm::dvec3& scale)
{
    SetTransform(translation, rotation, scale);
    ExtractLinearTransform();
}

RT::GTform::~GTform()
{

}

RT::GTform::GTform(const glm::dmat4x4& fwd, const glm::dmat4x4& bck)
{
    m_fwdtfm = fwd;
    m_bcktfm = bck;
    m_lintfm = glm::dmat3x3(1.0);
    ExtractLinearTransform();
}

void RT::GTform::SetTransform(const glm::dvec3& translation, const glm::dvec3& rotation, const glm::dvec3& scale)
{
    glm::dmat4x4 translationMatrix(1.0);

    glm::dmat4x4 rotationMatrixX(1.0);
    glm::dmat4x4 rotationMatrixY(1.0);
    glm::dmat4x4 rotationMatrixZ(1.0);
    glm::dmat4x4 scaleMatrix(1.0);

    translationMatrix[0][3] = translation[0];
    translationMatrix[1][3] = translation[1];
    translationMatrix[2][3] = translation[2];

    rotationMatrixZ[0][0] =  glm::cos(rotation[2]);
    rotationMatrixZ[0][1] = -glm::sin(rotation[2]);
    rotationMatrixZ[1][0] =  glm::sin(rotation[2]);
    rotationMatrixZ[1][1] =  glm::cos(rotation[2]);

    rotationMatrixY[0][0] =  glm::cos(rotation[1]);
    rotationMatrixY[0][2] =  glm::sin(rotation[1]);
    rotationMatrixY[2][0] = -glm::sin(rotation[1]);
    rotationMatrixY[2][2] =  glm::cos(rotation[1]);

    rotationMatrixX[1][1] =  glm::cos(rotation[0]);
    rotationMatrixX[1][2] = -glm::sin(rotation[0]);
    rotationMatrixX[2][1] =  glm::sin(rotation[0]);
    rotationMatrixX[2][2] =  glm::cos(rotation[0]);

    scaleMatrix[0][0] = scale[0];
    scaleMatrix[1][1] = scale[1];
    scaleMatrix[2][2] = scale[2];
    

    m_fwdtfm =  scaleMatrix *  
                rotationMatrixZ *
                rotationMatrixY *
                rotationMatrixX *
                translationMatrix;

    m_bcktfm = glm::inverse(m_fwdtfm);
}

glm::dmat4x4 RT::GTform::GetForward()
{
    return m_fwdtfm;
}

glm::dmat4x4 RT::GTform::GetBackward()
{
    return m_bcktfm;
}

RT::Ray RT::GTform::Apply(const Ray& inputRay, bool dirFlag)
{
    Ray outputRay;

    if (dirFlag) {
        outputRay.m_point1 = this->Apply(inputRay.m_point1, FWDTFORM);
        outputRay.m_point2 = this->Apply(inputRay.m_point2, FWDTFORM);

        outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
    }
    else {
        outputRay.m_point1 = this->Apply(inputRay.m_point1, BCKTFORM);
        outputRay.m_point2 = this->Apply(inputRay.m_point2, BCKTFORM);

        outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
    }

    return outputRay;
}

glm::dvec3 RT::GTform::Apply(const glm::dvec3& inputVector, bool dirFlag)
{   
    glm::dvec4 tempVector = glm::dvec4(inputVector, 1.0);

    glm::dvec4 resultVector(0.0);

    if (dirFlag) {
        resultVector = tempVector * m_fwdtfm;
    }
    else {
        resultVector = tempVector * m_bcktfm;
    }

    return glm::dvec3(resultVector);
}

glm::dvec3 RT::GTform::ApplyNorm(const glm::dvec3& inputVector)
{
    glm::dvec3 result = inputVector * m_lintfm;
    return result;
}

glm::dmat3x3 RT::GTform::GetNormalTransform()
{
    return m_lintfm;
}

RT::GTform RT::GTform::operator=(const GTform& rhs)
{
    if (this != &rhs) {
        m_fwdtfm = rhs.m_fwdtfm;
        m_bcktfm = rhs.m_bcktfm;
        m_lintfm = rhs.m_lintfm;
        ExtractLinearTransform();
    }
    return *this;
}

void RT::GTform::PrintMatrix(bool dirFlag)
{
    if (dirFlag) {
        Print(m_fwdtfm);
    }
    else {
        Print(m_bcktfm);
    }
}



void RT::GTform::Print(const glm::dmat3x3& matrix)
{
    for (int c = 0; c < matrix.length(); ++c) {
        for (int r = 0; r < matrix.length(); ++r) {
            std::cout << std::fixed << std::setprecision(3) << matrix[c][r] << " ";
        }
        std::cout << std::endl;
    }
     std::cout << std::endl;
}

void RT::GTform::Print(const glm::dmat4x4& matrix)
{
    for (int c = 0; c < matrix.length(); ++c) {
        for (int r = 0; r < matrix.length(); ++r) {
            std::cout << std::fixed << std::setprecision(3) << matrix[c][r] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void RT::GTform::ExtractLinearTransform()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m_lintfm[i][j] = m_fwdtfm[i][j];
        }
    }
    m_lintfm = glm::transpose(glm::inverse(m_lintfm));
}

namespace RT {
    RT::GTform operator*(const GTform& lhs, const GTform& rhs)
    {
        glm::dmat4x4 fwdResult = rhs.m_fwdtfm * lhs.m_fwdtfm;
        glm::dmat4x4 bckResult = glm::inverse(fwdResult);

        GTform finalResult(fwdResult, bckResult);

        return finalResult;
    }
}
