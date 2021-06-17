#pragma once
#include <memory>
#include "glm/glm.hpp"
#include "globjects/Framebuffer.h"
#include "globjects/Texture.h"
#include "globjects/RenderBuffer.h"
#include "modelstructure/ScreenMesh.h"

namespace otg {

	class VirtualScreen {

	public:
		VirtualScreen(std::int32_t width, std::int32_t height);

		void beginFrame();
		void endFrame(ShaderProgram& screenProgram);
		void draw(ShaderProgram& screenProgram);

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