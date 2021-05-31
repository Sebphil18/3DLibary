#include "globjects/CubeMapArray.h"
#include "glad/glad.h"
#include "glm/gtx/transform.hpp"
#include "globjects/RenderBuffer.h"
#include "modelstructure/CubeData.h"

namespace otg {

	const glm::mat4 CubeMapArray::projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	const glm::mat4 CubeMapArray::view[6] = {
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)),
	};

	CubeMapArray::CubeMapArray(const ImageFormat& imgFormat, std::uint32_t count) noexcept : 
		img({ nullptr, imgFormat.width, imgFormat.height, imgFormat.desiredChannels }), 
		count(count), levels(1), TextureTypes(TextureType::CubeMapArray)
	{
		init();
	}

	CubeMapArray::CubeMapArray(const ImageFormat& imgFormat, std::uint32_t levels, std::uint32_t count) noexcept :
		img({ nullptr, imgFormat.width, imgFormat.height, imgFormat.desiredChannels }),
		count(count), levels(levels), TextureTypes(TextureType::CubeMapArray)
	{
		init();
	}

	CubeMapArray::CubeMapArray(const CubeMapArray& other) noexcept :
		img(other.img), count(other.count), levels(other.levels), TextureTypes(other.type)
	{
		init();
	}

	CubeMapArray& CubeMapArray::operator=(const CubeMapArray& other) noexcept {

		img = other.img;
		count = other.count;
		type = other.type;

		deleteTexture();
		init();

		return *this;
	}

	CubeMapArray::~CubeMapArray() noexcept {
		deleteTexture();
	}

	void CubeMapArray::deleteTexture() {
		glDeleteTextures(1, &glHandle);
	}

	void CubeMapArray::init() {
		createTexture();
		setParameters();
	}

	void CubeMapArray::createTexture() {

		glCreateTextures(GL_TEXTURE_CUBE_MAP_ARRAY, 1, &glHandle);
		glTextureStorage3D(glHandle, levels, getGlFormat(type), img.width, img.height, 6 * count);
	}

	void CubeMapArray::setParameters() {

		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(glHandle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		if (levels == 1) {
			glTextureParameteri(glHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(glHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		} else
			enableMipmaps();
	}

	void CubeMapArray::enableMipmaps() {

		glGenerateTextureMipmap(glHandle);
		glTextureParameteri(glHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	CubeMapArray::CubeMapArray(CubeMapArray&& other) noexcept :
		img(std::move(other.img)),
		count(std::move(other.count)),
		levels(std::move(other.levels)),
		TextureTypes(std::move(other.type)),
		GlObject(std::move(other))
	{
	}

	CubeMapArray& CubeMapArray::operator=(CubeMapArray&& other) noexcept {
		
		GlObject::operator=(std::move(other));

		img = std::move(other.img);
		count = std::move(other.count);
		levels = std::move(other.levels);
		type = std::move(other.type);

		return *this;
	}

	void CubeMapArray::bindToUnit(std::uint32_t unit) {
		glBindTextureUnit(unit, glHandle);
	}

	void CubeMapArray::fromEquirectengular(const std::shared_ptr<HDRTexture>& equiRectTexture, ShaderProgram& conversionProgram) {

		std::int32_t viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glViewport(0, 0, img.width, img.height);

		convertEquirectengular(equiRectTexture, conversionProgram);

		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	}

	void CubeMapArray::convertEquirectengular(const std::shared_ptr<HDRTexture>& equiRectTexture, ShaderProgram& conversionProgram) {

		HDRTextureCube hdrCube(equiRectTexture);

		Framebuffer fbo;
		RenderBuffer rbo(img.width, img.height, TextureType::DepthStencilAttachment);

		fbo.attachRenderBuffer(rbo);

		renderToCube(hdrCube, fbo, conversionProgram);
	}

	void CubeMapArray::renderToCube(HDRTextureCube& hdrCube, Framebuffer& fbo, ShaderProgram& conversionProgram) {

		fbo.bind();
		for (std::uint32_t i = 0; i < 6; i++) {

			fbo.attachTextureLayer(glHandle, 0, i);
			fbo.clear();

			hdrCube.draw(conversionProgram, i);
		}
		fbo.unbind();
	}

	std::int32_t CubeMapArray::getWidth() const {
		return img.width;
	}

	std::int32_t CubeMapArray::getHeight() const {
		return img.height;
	}
}