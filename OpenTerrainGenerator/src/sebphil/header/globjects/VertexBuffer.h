#pragma once
#include <cstddef>
#include <cstdint>

namespace otg {

	class VertexBuffer {

	public:
		VertexBuffer() noexcept;
		VertexBuffer(std::size_t size, void* data, std::uint32_t usage) noexcept;
		VertexBuffer(const VertexBuffer& otherObj) noexcept;
		VertexBuffer(VertexBuffer&& otherObj) noexcept;

		VertexBuffer& operator=(const VertexBuffer& otherObj) noexcept;
		VertexBuffer& operator=(VertexBuffer&& otherObj) noexcept;

		~VertexBuffer() noexcept;

		void setData(std::size_t size, void* data, std::uint32_t usage);
		void use() const;
		static void unbind();

		std::uint32_t getGlHandle() const;

	private:
		std::uint32_t glHandle;
		std::uint32_t usage;
		std::size_t size;

		void* data;

		void createBuffer();
		void fillBuffer();
		void moveMember(VertexBuffer& otherObj) noexcept;

	};

}