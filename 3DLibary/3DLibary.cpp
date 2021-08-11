#include "stb/stbimage.h"
#include "application/Application.h"
#include "application/Scene.h"
#include "application/Stage.h"
#include "modelstructure/ModelLoader.h"
#include "modelstructure/Model.h"
#include "imgui/imgui.h"

// TODO: change Shader constructor: it should take in ready to use src-code and not a filePath so defines can be used
int main() {

	stbi_set_flip_vertically_on_load(true);

	glib::Application app;

	glib::Stage stage;
	std::shared_ptr<glib::Scene> scene = std::make_shared<glib::Scene>("rec/textures/hdr/outdoor/Helipad.hdr");
	stage.setScene(scene);

	glib::ModelLoader loader("rec/shapes/monkey/Monkey.fbx");
	glib::Model monkey(loader.getData());

	glib::Material material;
	auto albedoTex = std::make_shared<glib::TextureImage>("rec/textures/testtexture/TestGrid.png", glib::TextureType::Albedo);
	material.addTexture(albedoTex);

	monkey.meshes[0].setMaterial(material);

	auto& models = scene->models;
	models.push_back(monkey);

	stage.setOnUpdate([&](glib::Scene& scene, const glib::FrameClock& clock) {

		float angle = clock.getCurrentFrame() * 0.01;
		glm::vec3 rotation = glm::vec3(glm::radians(-90.0f), angle, 0);
		scene.models[0].setRotation(rotation);

		});

	float roughness = 0;
	float metallic = 0;
	float occlusion = 1;

	stage.setOnGuiDraw([&](glib::Scene& scene, const glib::FrameClock& clock) {

		ImGui::Begin("Material");
		if (ImGui::DragFloat("roughness", &roughness, 0.005, 0, 1)) {
			material.setRoughness(roughness);
			models[0].meshes[0].setMaterial(material);
		}
		if (ImGui::DragFloat("metallic", &metallic, 0.005, 0, 1)) {
			material.setMetallic(metallic);
			models[0].meshes[0].setMaterial(material);
		}
		if (ImGui::DragFloat("occlusion", &occlusion, 0.005, 0, 1)) {
			material.setOcclusion(occlusion);
			models[0].meshes[0].setMaterial(material);
		}
		ImGui::End();

		});

	stage.startRenderLoop();

	return 0;
}