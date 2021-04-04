#pragma once
#include <unordered_map>
#include <string>
#include <memory>

#include "globjects/TextureImage.h"

namespace otg {

	class TextureRegister {

	public:
		std::shared_ptr<TextureImage> getTexture(const std::string& filePath, TextureType type);

	private:
		typedef std::unordered_map<std::string, std::weak_ptr<TextureImage>>::iterator Iterator;

		std::unordered_map<std::string, std::weak_ptr<TextureImage>> registry;

		void generateNewTexture(const std::string& filePath, TextureType type, std::shared_ptr<TextureImage>& texture);
		void manageWeakPtr(Iterator itr, TextureType type, std::shared_ptr<TextureImage>& texture);

	};

}