#pragma once
#include <cstdint>
#include "globjects/GlObject.h"

namespace otg {
	
	class VertexArray : public GlObject {

	public:
		VertexArray() noexcept;
		VertexArray(VertexArray&& otherVertArray) noexcept;
		VertexArray& operator=(VertexArray&& otherVertArray) noexcept;

		VertexArray(const VertexArray& otherVertArray) = delete;
		VertexArray& operator=(const VertexArray& otherVertArray) = delete;
		
		~VertexArray() noexcept;

		void use();

	private:
		void createVertexArray();
		void deleteVertexArray();

	};

}