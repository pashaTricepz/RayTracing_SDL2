#include "pointlight.h"

RT::PointLight::PointLight()
{
    m_color = glm::dvec3(1.0);

    m_intensity = 1.0;
}

RT::PointLight::~PointLight()
{
    
}

bool RT::PointLight::ComputeIllumination(const glm::dvec3& intPoint, const glm::dvec3& localNormal,
                                         const std::vector<std::shared_ptr<ObjectBase>>& objectList,
                                         const std::shared_ptr<ObjectBase>& currentObject,
                                         glm::dvec3& color, double& intensity)
{
    glm::dvec3 lightDir = glm::normalize(m_location - intPoint);
    double lightDist = glm::length(m_location - intPoint);

    glm::dvec3 startPoint = intPoint + (localNormal * 0.001);

    glm::dvec3 endPoint = startPoint + lightDir;

    Ray lightRay(startPoint, endPoint);

    RT::DATA::hitData hitData;

    bool validInt = false;

    for (auto sceneObject : objectList) {
        if (sceneObject != currentObject) {
            validInt = sceneObject->TestIntersection(lightRay, hitData);
            if (validInt) {
                double dist = glm::length(hitData.poi - startPoint);
                if (dist > lightDist) {
                    validInt = false;
                }
            }
        }
        if (validInt)
            break;
    }

    if (!validInt) {
        double angle = acos(glm::dot(localNormal, lightDir));
        if (angle > (M_PI / 2.0)) {
            color = m_color;
            intensity = 0.0;
            return false;
        }
        else {
            color = m_color;
            intensity = m_intensity * (1.0 - (2.0 * angle / M_PI));
            return true;
        }
    }
    else {
        color = m_color;
        intensity = 0.0;
        return false;
    }
}
