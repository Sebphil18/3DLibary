#pragma once
#include <exception>
#include "modelstructure/ModelLoader.h"

namespace glib {

	class ModelLoadException : public std::exception {

	public:
		ModelLoadException(const ModelLoader& loader);

		const char* what() const override;

	private:
		std::string msg;

	};

}