#pragma once
#include <memory>
#include "glm/glm.hpp"
#include "globjects/Framebuffer.h"
#include "globjects/Texture.h"
#include "globjects/RenderBuffer.h"
#include "modelstructure/ScreenMesh.h"

namespace glib {

	class VirtualScreen {

	public:
		VirtualScreen(glm::ivec2 dimensions);
		VirtualScreen(std::int32_t width, std::int32_t height);

		void beginFrame();
		void endFrame(ShaderProgram& screenProgram);
		void draw();

		void setSize(std::int32_t width, std::int32_t height);
		std::uint32_t getScreenTexHandle() const;

	private:

		std::int32_t width, height;

		Framebuffer multiFbo;
		Framebuffer antialiasingFbo;
		Framebuffer screenFbo;

		MultisampleTexture multiTex;
		MultisampleRenderBuffer multiRbo;

		RenderBuffer antialiasingRbo;
		std::shared_ptr<Texture> antialiasingTex;

		RenderBuffer screenRbo;
		std::shared_ptr<Texture> screenTex;

		ScreenMesh screenMesh;

		void attachAttachments();

	};

}