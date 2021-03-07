#pragma once
#include <cstdint>
#include "globjects/GlObject.h"

namespace otg {
	
	class VertexArray : public GlObject {

	public:
		VertexArray() noexcept;
		/// <summary>
		/// This copy contructor does not copy the vertex array but creates a new one 
		/// because the old one might get deleted even if the copy is in use (see destructor).
		/// </summary>
		VertexArray(const VertexArray& otherVao) noexcept;
		VertexArray(VertexArray&& otherVao) noexcept;

		VertexArray& operator=(const VertexArray& otherVao) noexcept;
		VertexArray& operator=(VertexArray&& otherVao) noexcept;
		
		~VertexArray() noexcept;

		void use();

	private:
		void createVertexArray();
		void deleteVertexArray();

	};

}