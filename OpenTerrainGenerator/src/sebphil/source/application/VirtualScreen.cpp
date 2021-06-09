#include "application/VirtualScreen.h"

namespace otg {

	VirtualScreen::VirtualScreen(std::int32_t width, std::int32_t height) :
		width(width), height(height),
		multiTex(width, height, 8),
		multiRbo(width, height, 8),
		screenTex(std::make_shared<Texture>(width, height, TextureType::ColorAttachment)), 
		screenRbo(width, height, TextureType::DepthStencilAttachment)
	{
		attachAttachments();
		screenMesh.addTexture(screenTex);
	}

	void VirtualScreen::setSize(std::int32_t width, std::int32_t height) {

		this->width = width;
		this->height = height;

		screenTex->setSize(width, height);
		screenRbo.setSize(width, height);

		multiTex.setSize(width, height);
		multiRbo.setSize(width, height);

		attachAttachments();
	}

	void VirtualScreen::attachAttachments() {

		multiFbo.attachTexture(multiTex);
		multiFbo.attachRenderBuffer(multiRbo);

		screenFbo.attachTexture(*screenTex);
		screenFbo.attachRenderBuffer(screenRbo);
	}

	void VirtualScreen::beginFrame() {
		multiFbo.clear();
		multiFbo.bind();
	}

	void VirtualScreen::endFrame() {
		multiFbo.unbind();
		screenFbo.clear();
		multiFbo.copyColorTo(width, height, screenFbo);
	}

	void VirtualScreen::draw(ShaderProgram& screenProgram) {
		screenMesh.draw(screenProgram);
	}

}