#pragma once
#include "application/ShaderProgramRegister.h"
#include "application/MatrixUniformBuffer.h"
#include "camera/OrbitCamera.h"
#include "lighting/SkyBox.h"
#include "lighting/SkyLight.h"

namespace glib {

	class Scene {

	public:
		glib::ShaderProgramRegister programs;
		glib::MatrixUniformBuffer matrices;
		glib::OrbitCamera cam;

		glib::SkyBox skyBox;
		glib::SkyLight skyLight;

		std::vector<glib::Model> models;

		Scene() noexcept;
		Scene(const std::string& skyMapFilePath, int width = 1200, int height = 800) noexcept;
		Scene(const Scene& other) noexcept;

		Scene& operator=(const Scene& other) noexcept;

		void update();
		void draw();

		void setSize(int width, int height);
		void setHDRMap(const std::string& filePath);

	private:
		void bind();
		void drawModels();
	};

}