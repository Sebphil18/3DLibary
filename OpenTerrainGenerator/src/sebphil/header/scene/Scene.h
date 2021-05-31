#pragma once
#include <vector>
#include <memory>

#include "modelstructure/Model.h"
#include "camera/Camera.h"

namespace otg {

	struct Scene {
		Camera cam;
		std::vector<std::shared_ptr<Model>> models;
		std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> programs;
		// TODO: lights
	};

}