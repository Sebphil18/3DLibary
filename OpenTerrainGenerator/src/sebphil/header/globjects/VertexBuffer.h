#pragma once
#include <cstddef>
#include <cstdint>
#include "globjects/GlObject.h"

namespace otg {

	class VertexBuffer : public GlObject {

	public:
		VertexBuffer() noexcept;
		VertexBuffer(std::size_t size, void* data, std::uint32_t usage) noexcept;
		VertexBuffer(const VertexBuffer& otherVbo) noexcept;
		VertexBuffer(VertexBuffer&& otherVbo) noexcept;

		VertexBuffer& operator=(const VertexBuffer& otherVbo) noexcept;
		VertexBuffer& operator=(VertexBuffer&& otherVbo) noexcept;

		~VertexBuffer() noexcept;

		void setData(std::size_t size, void* data, std::uint32_t usage);
		void updateData(std::uint32_t size, std::uint32_t offset, void* data);

		void use() const;
		static void unbind();

	private:
		std::uint32_t usage;
		std::size_t size;

		void* data;

		void init();

		void createBuffer();
		void fillBuffer();

	};

}