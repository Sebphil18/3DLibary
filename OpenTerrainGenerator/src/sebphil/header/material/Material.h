#pragma once
#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "globjects/Texture.h"
#include "globjects/ShaderProgram.h"

namespace otg {

	struct DeferredMaterial {

		float roughness, metallic, occlusion;
		glm::vec3 albedo, specular, ambient;
		std::vector<DeferredTexture> texturePaths;
	};

	class Material {

	public:
		Material();
		Material(const std::vector<std::shared_ptr<Texture>>& textures);

		void addTexture(const std::shared_ptr<Texture>& texture);
		void removeTexture(std::size_t index);
		void clearTextures();
		std::shared_ptr<Texture> getTexture(std::size_t index);

		void setUniforms(ShaderProgram& program);
		void bindTexture(ShaderProgram& program);
		void unbindTextures(ShaderProgram& program);
		void setTextures(const std::vector<std::shared_ptr<Texture>>& textures);

		void setMetallic(float metallic);
		void setRoughness(float roguhness);
		void setOcclusion(float occlusion);

		void setAlbedo(glm::vec3 albedo);
		void setSpecular(glm::vec3 specular);
		void setAmbient(glm::vec3 ambient);

	private:

		struct TextureIterators {
			std::uint32_t albedoTextures = 0;
			std::uint32_t roughnessTextures = 0;
			std::uint32_t occlussionTextures = 0;
			std::uint32_t metallicTextures = 0;
			std::uint32_t displacementTextures = 0;
			std::uint32_t normalTextures = 0;
		};

		float roughness, metallic, occlusion;
		glm::vec3 albedo, specular, ambient;
		std::vector<std::shared_ptr<Texture>> textures;

		std::string getTexUniName(TextureIterators& iterators, TextureType type);
		void setTexUniName(std::uint32_t& iterator, const std::string& uniPrefix, std::string& uniName);

	};

}