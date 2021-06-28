#include <iostream>
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
	material.setAlbedo(glm::vec3(1, 0, 0));
	material.setRoughness(0.2);
	material.setMetallic(0);

	monkey.meshes[0].setMaterial(material);
	monkey.setRotation(glm::vec3(glm::radians(-90.0f), 0, 0));

	auto& models = scene->models;
	models.push_back(monkey);

	stage.startRenderLoop();

	return 0;
}