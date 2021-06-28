# 3DLibary
This project uses the libaries: GLFW (https://www.glfw.org/), GLM (https://github.com/g-truc/glm), Assimp (https://www.assimp.org/), ImGui (https://github.com/ocornut/imgui), stb (https://github.com/nothings/stb). <br>
simple program that displays simple model:
```c++
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
```
output: <br>
<image src="https://i.imgur.com/9BphM7J.png"> <br>
