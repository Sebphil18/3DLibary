#include "modelstructure/TextureRegister.h"

namespace otg {

	/// <summary>
	/// This function should be used to register a new TextureImage or to retreive a already loaded one.
	/// If a TextureImage with the corresponding filepath is already registered, no new TextureImage will be created.
	//	This function should prevent unnecessary creation of images (and textures) that already got loaded.
	/// </summary>
	/// <param name="filePath">The unique filepath of the picture that should be loaded into a texture</param>
	/// <param name="type">The texture type, which is used when a new TextureImage needs to be generated</param>
	/// <returns>Returns a shared_ptr of a TextureImage, which can be used for models etc.</returns>
	std::shared_ptr<TextureImage> TextureRegister::getTexture(const std::string& filePath, TextureType type) {

		Iterator itr = registry.find(filePath);

		std::shared_ptr<TextureImage> texture;

		if (itr == registry.end())
			generateNewTexture(filePath, type, texture);
		else
			manageWeakPtr(itr, type, texture);

		return texture;
	}

	void TextureRegister::generateNewTexture(const std::string& filePath, TextureType type, std::shared_ptr<TextureImage>& texture) {
		texture = std::make_shared<TextureImage>(filePath, type);
		registry[filePath] = texture;
	}

	void otg::TextureRegister::manageWeakPtr(Iterator itr, TextureType type, std::shared_ptr<TextureImage>& texture) {

		std::weak_ptr<TextureImage> weakTexture = itr->second;

		if (weakTexture.expired()) {
			generateNewTexture(itr->first, type, texture);
		} else
			texture = std::shared_ptr<TextureImage>(weakTexture);
	}
}