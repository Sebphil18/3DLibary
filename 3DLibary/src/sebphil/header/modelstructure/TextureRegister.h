#pragma once
#include <unordered_map>
#include <string>
#include <memory>

#include "globjects/TextureImage.h"

namespace glib {

	class TextureRegister {

	public:

		/// <summary>
		/// This function should be used to register a new TextureImage or to retreive a already loaded one.
		/// If a TextureImage with the corresponding filepath is already registered, no new TextureImage will be created.
		//	This function should prevent unnecessary creation of images (and textures) that already got loaded.
		/// </summary>
		/// <param name="filePath">The unique filepath of the picture that should be loaded into a texture</param>
		/// <param name="type">The texture type, which is used when a new TextureImage needs to be generated</param>
		/// <returns>Returns a shared_ptr of a TextureImage, which can be used for models etc.</returns>
		std::shared_ptr<TextureImage> loadTexture(const std::string& filePath, TextureType type);

	private:
		typedef std::unordered_map<std::string, std::weak_ptr<TextureImage>>::iterator Iterator;

		std::unordered_map<std::string, std::weak_ptr<TextureImage>> registry;

		void generateNewTexture(const std::string& filePath, TextureType type, std::shared_ptr<TextureImage>& texture);
		void manageWeakPtr(Iterator itr, TextureType type, std::shared_ptr<TextureImage>& texture);

	};

}