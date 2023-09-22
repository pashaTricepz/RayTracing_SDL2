#pragma once

#include "scene.h"

namespace RT {
	class ExampleScene : public Scene {
	public:
		ExampleScene();
		virtual ~ExampleScene();

		virtual void SetupSceneObjects() override;
	};
}