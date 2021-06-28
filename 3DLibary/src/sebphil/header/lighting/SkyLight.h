#pragma once
#include "lighting/SkyBoxLightProbe.h"
#include "lighting/SkyBoxReflectionProbe.h"
#include "globjects/CubeMapArray.h"
#include "application/ShaderProgramRegister.h"

namespace glib {

	class SkyLight {

	public:
		SkyLight(glib::CubeMapArray& skyMap, glib::ShaderProgramRegister& programs);
		SkyLight(const std::string& skyMapFilePath, glib::ShaderProgramRegister& programs);
		SkyLight(const SkyLight& other);

		SkyLight& operator=(const SkyLight& other);

		void bindTo(glib::ShaderProgram& program);
		void bindTextureUnits();
		void setUniforms(glib::ShaderProgram& program);

		void bake(const std::string& skyMapFilePath, glib::ShaderProgram& conversionProgram);

	private:
		std::string skyMapFilePath;
		glib::ShaderProgramRegister& programs;
		glib::CubeMapArray skyMap;
		glib::SkyBoxLightProbe lightProbe;
		glib::SkyBoxReflectionProbe reflectionProbe;
	};
}