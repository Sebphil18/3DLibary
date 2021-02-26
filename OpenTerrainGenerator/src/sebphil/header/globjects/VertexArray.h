#pragma once
#include <cstdint>

namespace otg {
	
	class VertexArray {

	public:
		VertexArray() noexcept;
		VertexArray(VertexArray&& otherVertArray) noexcept;
		VertexArray& operator=(VertexArray&& otherVertArray) noexcept;

		VertexArray(const VertexArray& otherVertArray) = delete;
		VertexArray& operator=(const VertexArray& otherVertArray) = delete;
		
		~VertexArray() noexcept;

		void use();

		std::uint32_t getGlHandle() const;

	private:
		std::uint32_t glHandle;

		void createVertexArray();
		void deleteVertexArray();

	};

}