#pragma once
#include <vector>
#include "globjects/GlObject.h"
#include "globjects/UniformType.h"
#include "globjects/ShaderProgram.h"

namespace otg{

	struct UniformElement {
		UniformType type;
		void* data = nullptr;
		std::uint32_t size = UniformTypes::getSize(type);
		std::uint32_t offset = 0;
	};

	class UniformBuffer : public GlObject {

	public:	
		UniformBuffer() noexcept;
		UniformBuffer(std::uint32_t capacity) noexcept;
		UniformBuffer(const UniformBuffer& other) noexcept;
		UniformBuffer(UniformBuffer&& other) noexcept;

		UniformBuffer& operator=(const UniformBuffer& other) noexcept;
		UniformBuffer& operator=(UniformBuffer&& other) noexcept;

		~UniformBuffer() noexcept;

		void addElement(const UniformElement& element);
		void setElementData(std::uint32_t index, void* data);

		void bindTo(ShaderProgram& program, const std::string& blockName, std::uint32_t bindingPoint);

	private:
		std::uint32_t size;
		std::uint32_t capacity;
		std::vector<UniformElement> elements;

		void createRenderBuffer();
		void allocateMemory();

		bool isTooSmall();
		void adjustToSize();
		void fillBuffer();
		void setElementData(const UniformElement& element);

		std::uint32_t getBlockIndex(const ShaderProgram& program, const std::string& blockName);

	};

}
