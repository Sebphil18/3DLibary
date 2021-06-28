#include "application/VirtualScreen.h"

namespace glib {

	VirtualScreen::VirtualScreen(glm::ivec2 dimensions) :
		VirtualScreen(dimensions.x, dimensions.y)
	{
	}

	VirtualScreen::VirtualScreen(std::int32_t width, std::int32_t height) :
		width(width), height(height),
		multiTex(width, height, 8),
		multiRbo(width, height, 8),
		antialiasingTex(std::make_shared<Texture>(width, height, TextureType::ColorAttachment)), 
		antialiasingRbo(width, height, TextureType::DepthStencilAttachment),
		screenTex(std::make_shared<Texture>(width, height, TextureType::ColorAttachment)),
		screenRbo(width, height, TextureType::DepthStencilAttachment)
	{
		attachAttachments();
		screenMesh.addTexture(antialiasingTex);
	}

	void VirtualScreen::setSize(std::int32_t width, std::int32_t height) {

		this->width = width;
		this->height = height;

		antialiasingTex->setSize(width, height);
		antialiasingRbo.setSize(width, height);

		multiTex.setSize(width, height);
		multiRbo.setSize(width, height);

		screenTex->setSize(width, height);
		screenRbo.setSize(width, height);

		attachAttachments();
	}

	void VirtualScreen::attachAttachments() {

		multiFbo.attachTexture(multiTex);
		multiFbo.attachRenderBuffer(multiRbo);

		antialiasingFbo.attachTexture(*antialiasingTex);
		antialiasingFbo.attachRenderBuffer(antialiasingRbo);

		screenFbo.attachTexture(*screenTex);
		screenFbo.attachRenderBuffer(screenRbo);
	}

	void VirtualScreen::beginFrame() {
		multiFbo.clear();
		multiFbo.bind();
	}

	void VirtualScreen::endFrame(ShaderProgram& screenProgram) {
		multiFbo.unbind();
		antialiasingFbo.clear();
		multiFbo.copyTo(width, height, antialiasingFbo);

		screenFbo.clear();
		screenFbo.bind();
		screenMesh.draw(screenProgram);
		screenFbo.unbind();
	}

	void VirtualScreen::draw() {
		screenFbo.copyTo(width, height, 0);
	}

	std::uint32_t VirtualScreen::getScreenTexHandle() const {
		return screenTex->getGlHandle();
	}

}