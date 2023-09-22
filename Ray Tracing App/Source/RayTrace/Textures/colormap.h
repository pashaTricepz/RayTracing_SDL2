#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace RT {
	namespace Texture {
		class ColorMap {
		public:
			ColorMap();
			~ColorMap();

			void SetStop(double position, const glm::dvec4& value);

			glm::dvec4 GetColor(double position);

		private:
			std::vector<double> m_stopPositions;
			std::vector<glm::dvec4> m_stopValues;
		};
	}
}