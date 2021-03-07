#pragma once
#include <cstdint>
#include <cstddef>
#include "GlObject.h"

namespace otg {

	class IndexBuffer : public GlObject{
	
	public:
		IndexBuffer() noexcept;
		IndexBuffer(std::size_t size, void* data, std::uint32_t usage) noexcept;
		IndexBuffer(const IndexBuffer& otherIbo) noexcept;
		IndexBuffer(IndexBuffer&& otherIbo) noexcept;

		IndexBuffer& operator=(const IndexBuffer& otherIbo) noexcept;
		IndexBuffer& operator=(IndexBuffer && otherIbo) noexcept;

		~IndexBuffer() noexcept;

		void setData(std::size_t size, void* data, std::uint32_t usage);

		std::size_t getElementCount() const;

	private:
		std::uint32_t usage;
		std::size_t size;

		void* data;
		
		void copyMembers(const IndexBuffer& otherIbo);
		void moveMembers(IndexBuffer& otherIbo);
		void createBuffer();
		void fillBuffer();
		void deleteBuffer();
	
	};

}