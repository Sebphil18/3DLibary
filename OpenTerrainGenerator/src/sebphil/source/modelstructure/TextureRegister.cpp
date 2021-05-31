#include "modelstructure/TextureRegister.h"

namespace otg {

	std::shared_ptr<TextureImage> TextureRegister::loadTexture(const std::string& filePath, TextureType type) {

		Iterator itr = registry.find(filePath);

		std::shared_ptr<TextureImage> texture;

		if (itr == registry.end())
			generateNewTexture(filePath, type, texture);
		else
			manageWeakPtr(itr, type, texture);

		return texture;
	}

	void otg::TextureRegister::manageWeakPtr(Iterator itr, TextureType type, std::shared_ptr<TextureImage>& texture) {

		std::weak_ptr<TextureImage> weakTexture = itr->second;

		if (weakTexture.expired()) {
			generateNewTexture(itr->first, type, texture);
		} else
			texture = std::shared_ptr<TextureImage>(weakTexture);
	}

	void TextureRegister::generateNewTexture(const std::string& filePath, TextureType type, std::shared_ptr<TextureImage>& texture) {
		texture = std::make_shared<TextureImage>(filePath, type);
		registry[filePath] = texture;
	}
}