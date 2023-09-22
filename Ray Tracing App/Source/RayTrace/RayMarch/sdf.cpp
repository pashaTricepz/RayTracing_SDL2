#include "sdf.h"

double RT::RM::SDF::Sphere(const glm::dvec3& p, const glm::dvec3& center, const glm::dvec3& parms)
{
    return glm::length(p - center) - parms[0];
}

double RT::RM::SDF::Torus(const glm::dvec3& p, const glm::dvec3& center, const glm::dvec3& parms)
{
    double x = p[0] - center[0];
    double y = p[1] - center[1];
    double z = p[2] - center[2];

    double t1 = sqrt(x * x + y * y) - parms[0];
    double t2 = sqrt(t1 * t1 + z * z) - parms[1];

    return t2;
}

double RT::RM::SDF::Box(const glm::dvec3& p, const glm::dvec3& center, const glm::dvec3& parms)
{
    glm::dvec3 location = p - center;

    double ax = fabs(location[0]) - parms[0];
    double ay = fabs(location[1]) - parms[1];
    double az = fabs(location[2]) - parms[2];

    double bx = glm::max(ax, 0.0);
    double by = glm::max(ay, 0.0);
    double bz = glm::max(az, 0.0);

    double internalDist = glm::min(glm::max(ax, glm::max(ay, az)), 0.0);
    double externalDist = sqrt(bx * bx + by * by + bz * bz);

    return internalDist + externalDist;
}
