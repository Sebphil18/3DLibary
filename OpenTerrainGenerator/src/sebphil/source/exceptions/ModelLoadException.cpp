#include "exceptions/ModelLoadException.h"

namespace otg {

	ModelLoadException::ModelLoadException(const ModelLoader& loader) {
		msg = "ERROR::MODELLOADER::Could not load model/scene '" + loader.getFilePath() + "'!";
	}

	const char* ModelLoadException::what() const {
		return msg.c_str();
	}

}