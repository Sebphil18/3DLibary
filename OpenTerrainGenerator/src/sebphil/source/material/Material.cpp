#include "material/Material.h"

namespace otg {
	
	Material::Material() :
		roughness(0.5), metallic(0.5), occlusion(1),
		albedo(1, 0, 1), specular(1, 1, 0), ambient(0, 1, 1)
	{
	}

	Material::Material(const std::vector<std::shared_ptr<Texture>>& textures) :
		roughness(0.1), metallic(0.2), occlusion(1),
		albedo(1, 0, 1), specular(1, 1, 0), ambient(0, 1, 1),
		textures(textures)
	{
	}

	void Material::addTexture(const std::shared_ptr<Texture>& texture) {
		textures.push_back(texture);
	}

	void Material::removeTexture(std::size_t index) {
		textures.erase(textures.begin() + index);
	}

	void Material::clearTextures() {
		textures.clear();
	}

	std::shared_ptr<Texture> Material::loadTexture(std::size_t index) {
		return textures[index];
	}

	void Material::bindTextures(const std::vector<std::shared_ptr<Texture>>& textures) {
		this->textures = textures;
	}

	void Material::setUniforms(ShaderProgram& program) {

		program.setUniform("material.roughness", roughness);
		program.setUniform("material.metallic", metallic);
		program.setUniform("material.occlusion", occlusion);
		program.setUniformVec("material.albedoColor", albedo);
		program.setUniformVec("material.specularColor", specular);
		program.setUniformVec("material.ambientColor", ambient);
	}

	void Material::bindTexture(ShaderProgram& program) {

		TextureIterators iterators;

		for (std::int32_t unit = 1; unit <= static_cast<int>(textures.size()); unit++) {

			const std::shared_ptr<Texture>& texture = textures[static_cast<std::size_t>(unit) - 1];

			texture->bindToUnit(unit);

			std::string uniName = getTexUniName(iterators, texture->getType());
			program.setUniform(uniName, unit);
		}
	}

	void Material::unbindTextures(ShaderProgram& program) {

		TextureIterators iterators;

		for (std::int32_t unit = 0; unit < static_cast<int>(textures.size()); unit++) {

			const std::shared_ptr<Texture>& texture = textures[static_cast<std::size_t>(unit)];

			std::string uniName = getTexUniName(iterators, texture->getType());
			program.setUniform(uniName, 0);
		}
	}

	std::string Material::getTexUniName(TextureIterators& iterators, otg::TextureType type) {

		std::string uniName = "";

		switch (type) {

		case TextureType::Albedo: setTexUniName(iterators.albedoTextures, "material.albedoTex", uniName);
			break;
		case TextureType::Roughness: setTexUniName(iterators.roughnessTextures, "material.roughnessTex", uniName);
			break;
		case TextureType::Metallic: setTexUniName(iterators.metallicTextures, "material.metallicTex", uniName);
			break;
		case TextureType::Occlusion: setTexUniName(iterators.occlussionTextures, "material.occlusionTex", uniName);
			break;
		case TextureType::Displacement: setTexUniName(iterators.displacementTextures, "material.displacementTex", uniName);
			break;
		case TextureType::Normal: setTexUniName(iterators.normalTextures, "material.normalTex", uniName);
			break;
		default:
			setTexUniName(iterators.albedoTextures, "material.albedoTex", uniName);
		}

		return uniName;
	}

	void Material::setTexUniName(std::uint32_t& iterator, const std::string& uniPrefix, std::string& uniName) {
		uniName = uniPrefix + std::to_string(iterator);
		iterator++;
	}

	void Material::setMetallic(float metallic) {
		this->metallic = metallic;
	}

	void Material::setRoughness(float roughness) {
		this->roughness = roughness;
	}

	void Material::setOcclusion(float occlusion) {
		this->occlusion = occlusion;
	}

	void Material::setAlbedo(glm::vec3 albedo) {
		this->albedo = albedo;
	}

	void Material::setSpecular(glm::vec3 specular) {
		this->specular = specular;
	}

	void Material::setAmbient(glm::vec3 ambient) {
		this->ambient = ambient;
	}

}