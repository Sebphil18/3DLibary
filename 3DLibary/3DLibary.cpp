#include "stb/stbimage.h"
#include "application/Application.h"
#include "application/Scene.h"
#include "application/Stage.h"
#include "modelstructure/ModelLoader.h"
#include "modelstructure/Model.h"

// test app
int main() {
	
	stbi_set_flip_vertically_on_load(true);

	glib::Application app;

	glib::Stage stage;
	std::shared_ptr<glib::Scene> scene = std::make_shared<glib::Scene>("rec/textures/hdr/outdoor/Helipad.hdr");
	stage.setScene(scene);

	glib::ModelLoader loader("rec/shapes/monkey/Monkey.fbx");
	glib::Model monkey(loader.getData());

	glib::Material material;
	auto texture = std::make_shared<glib::TextureImage>("rec/textures/testtexture/TestGrid.png", glib::TextureType::Albedo);
	material.addTexture(texture);

	monkey.meshes[0].setMaterial(material);
	monkey.setRotation(glm::vec3(glm::radians(-90.0f), 0, 0));

	auto& models = scene->models;
	models.push_back(monkey);

	stage.setOnUpdate([](glib::Scene& scene, const glib::FrameClock& clock) {

		float angle = clock.getCurrentFrame() * 0.01;
		glm::vec3 rotation = glm::vec3(angle, angle, 0);
		scene.models[0].setRotation(rotation);

		});

	stage.startRenderLoop();

	return 0;
}