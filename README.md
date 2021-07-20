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
adding a texture to the material:
```c++
glib::Material material;

auto texture = std::make_shared<glib::TextureImage>("rec/textures/testtexture/TestTexture.png", glib::TextureType::Albedo);
material.addTexture(texture);

monkey.meshes[0].setMaterial(material);
```
TextureTypes that might be useful: Albedo, Roughness, Metallic, Occlusion, Normal <br>
Please note that the textures are added to the other corresponding material properties. For example, if the material has a roughness of 0.5 and the roughness-texture a value of 0.2 the resulting roughness will be 0.7. Every texture has to be at least rgb.<br>
