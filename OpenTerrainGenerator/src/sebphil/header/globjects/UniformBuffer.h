#pragma once
#include <vector>
#include "globjects/GlObject.h"
#include "globjects/UniformType.h"
#include "globjects/ShaderProgram.h"

namespace otg{

	struct UniformElement {
		UniformType type;
		void* data;
		std::uint32_t size = UniformTypes::getSize(type);
	};

	class UniformBuffer : public GlObject {

	public:	
		UniformBuffer() noexcept;
		UniformBuffer(const UniformBuffer& other) noexcept;
		UniformBuffer(UniformBuffer&& other) noexcept;

		UniformBuffer& operator=(const UniformBuffer& other) noexcept;
		UniformBuffer& operator=(UniformBuffer&& other) noexcept;

		~UniformBuffer() noexcept;

		void addElement(const UniformElement& element);
		void setElement(std::size_t index, UniformElement element);
		void removeElement(std::size_t index);

		void fillBuffer();

		void bindTo(ShaderProgram& program, const std::string& blockName, std::uint32_t bindingPoint);

	private:
		std::uint32_t size;
		std::vector<UniformElement> elements;

		void createRenderBuffer();
		std::uint32_t getBlockIndex(const ShaderProgram& program, const std::string& blockName);

	};

}
