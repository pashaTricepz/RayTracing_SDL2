#include "utils.h"

void RT::UTILS::PrintVector(const glm::dvec3& inputVector)
{
    for (int r = 0; r < inputVector.length(); ++r) {
        std::cout << std::fixed << std::setprecision(3) << inputVector[r] << ' ';
    }
    std::cout << std::endl;
}
