#include "lighting/SkyLight.h"

namespace glib {

	SkyLight::SkyLight(glib::CubeMapArray& skyMap, glib::ShaderProgramRegister& programs) :
		programs(programs),
		lightProbe(skyMap, programs["convolution"]),
		reflectionProbe(skyMap, programs["prefilter"], programs["brdfIntegration"]),
		skyMap(skyMap) {
	};

	SkyLight::SkyLight(const std::string& skyMapFilePath, glib::ShaderProgramRegister& programs) :
		programs(programs),
		skyMapFilePath(skyMapFilePath),
		skyMap({ 512, 512 }, skyMapFilePath, * programs["equirectConversion"]),
		lightProbe(skyMap, programs["convolution"]),
		reflectionProbe(skyMap, programs["prefilter"], programs["brdfIntegration"]) {
	}

	SkyLight::SkyLight(const SkyLight& other) :
		programs(other.programs),
		skyMapFilePath(other.skyMapFilePath),
		skyMap({ 512, 512 }, skyMapFilePath, * programs["equirectConversion"]),
		lightProbe(skyMap, programs["convolution"]),
		reflectionProbe(skyMap, programs["prefilter"], programs["brdfIntegration"]) {
	}

	SkyLight& SkyLight::operator=(const SkyLight& other) {

		programs = other.programs;
		skyMapFilePath = other.skyMapFilePath;

		std::shared_ptr<glib::HDRTexture> hdrTexture = std::make_shared<glib::HDRTexture>(skyMapFilePath);
		skyMap.fromEquirectengular(hdrTexture, *programs["equirectConversion"]);

		lightProbe = glib::SkyBoxLightProbe(skyMap, programs["convolution"]);
		reflectionProbe = glib::SkyBoxReflectionProbe(skyMap, programs["prefilter"], programs["brdfIntegration"]);

		return *this;
	}

	void SkyLight::bindTo(glib::ShaderProgram& program) {
		bindTextureUnits();
		setUniforms(program);
	}

	void SkyLight::bindTextureUnits() {
		skyMap.bindToUnit(10);
		lightProbe.bindToUnit(11);
		reflectionProbe.bindPrefilterToUnit(12);
		reflectionProbe.bindLookUpTexToUnit(13);
	}

	void SkyLight::setUniforms(glib::ShaderProgram& program) {
		program.setUniform("envMap", 11);
		program.setUniform("prefilterMap", 12);
		program.setUniform("brdf", 13);
	}

	void SkyLight::bake(const std::string& skyMapFilePath, glib::ShaderProgram& conversionProgram) {

		this->skyMapFilePath = skyMapFilePath;

		std::shared_ptr<glib::HDRTexture> hdrTexture = std::make_shared<glib::HDRTexture>(skyMapFilePath);
		skyMap.fromEquirectengular(hdrTexture, conversionProgram);
	}

}