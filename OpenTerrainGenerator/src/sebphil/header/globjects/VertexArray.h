#pragma once
#include <cstdint>

namespace otg {
	
	// TODO: maybe delete assignment and copying because user may expect that layout is copied aswell
	class VertexArray {

	public:
		VertexArray() noexcept;
		VertexArray(const VertexArray& otherVertArray) noexcept;
		VertexArray(VertexArray&& otherVertArray) noexcept;

		VertexArray& operator=(const VertexArray& otherVertArray) noexcept;
		VertexArray& operator=(VertexArray&& otherVertArray) noexcept;
		
		~VertexArray() noexcept;

		void bind();

		std::uint32_t getGlHandle() const;

	private:
		std::uint32_t glHandle;

		void createVertexArray();
		void deleteVertexArray();

	};

}