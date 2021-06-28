#include "application/Scene.h"

namespace glib {

	Scene::Scene() noexcept :
		skyLight("", programs) {
	}

	Scene::Scene(const std::string& skyMapFilePath, int width, int height) noexcept :
		skyLight(skyMapFilePath, programs),
		cam({ width, height }) {
	}

	Scene::Scene(const Scene& other) noexcept :
		skyBox(other.skyBox),
		skyLight(other.skyLight),
		matrices(other.matrices),
		cam(other.cam),
		models(other.models) {
	}

	Scene& Scene::operator=(const Scene& other) noexcept {

		skyBox = other.skyBox;
		skyLight = other.skyLight;
		matrices = other.matrices;
		cam = other.cam;
		models = other.models;

		return *this;
	}

	void Scene::update() {
		matrices.setCamMatrices(cam);
		programs["main"]->setUniformVec("viewPos", cam.getPosition());
		programs["main"]->setUniformVec("lightPos", glm::vec3(-1, 1, 1) * 100.0f);
	}

	void Scene::draw() {
		bind();
		drawModels();
		skyBox.draw(cam, *programs["skyBox"]);
	}

	void Scene::bind() {
		matrices.bindTo(0);
		skyLight.bindTo(*programs["main"]);
	}

	void Scene::drawModels() {

		for (glib::Model& model : models) {
			matrices.setModelMatrices(model);
			model.draw(*programs["main"]);
		}
	}

	void Scene::setSize(int width, int height) {
		cam.setSize(width, height);
	}

	void Scene::setHDRMap(const std::string& filePath) {
		skyLight.bake(filePath, *programs["equirectConversion"]);
	}

}