#pragma once
#include <vector>
#include "glm/glm.hpp"

namespace otg {

	template<typename T>
	class Heightmap {

	public:
		Heightmap(glm::ivec2 dimensions);

		void setSize(glm::ivec2 newSize);
		void setValue(glm::ivec2 cell, T value);

		T getValue(glm::ivec2 cell) const;
		glm::ivec2 getSize() const;

		std::uint32_t getIndex(glm::ivec2 cell) const;

	private:
		glm::ivec2 dimensions;
		std::vector<T> values;

		glm::ivec2 getMinSize(glm::ivec2 newSize) const;
		void copyValues(glm::ivec2 minSize, glm::ivec2 newSize, const std::vector<T>& temp);

	};

	template<typename T>
	inline Heightmap<T>::Heightmap(glm::ivec2 dimensions) : 
		dimensions(dimensions), values(dimensions.x * dimensions.y, 0)
	{
	}

	template<typename T>
	inline void Heightmap<T>::setSize(glm::ivec2 newSize) {

		values.clear();
		values.reserve(newSize.x * newSize.y);

		glm::ivec2 minSize = getMinSize(newSize);
		std::vector<T> temp = values;

		copyValues(minSize, newSize, temp);
		
		values = temp;
		dimensions = newSize;
	}

	template<typename T>
	inline glm::ivec2 Heightmap<T>::getMinSize(glm::ivec2 newSize) const {

		return {
			std::min(dimensions.x, newSize.x),
			std::min(dimensions.y, newSize.y)
		};
	}

	template<typename T>
	inline void Heightmap<T>::copyValues(glm::ivec2 minSize, glm::ivec2 newSize, const std::vector<T>& temp) {

		for (std::uint32_t x = 0; x < minSize.x; x++) {
			for (std::uint32_t y = 0; y < minSize.y; y++) {

				std::uint32_t indexNew = y * newSize.x + x;
				std::uint32_t indexOld = y * dimensions.x + x;
				values[indexOld] = temp[indexNew];
			}
		}
	}

	template<typename T>
	inline void Heightmap<T>::setValue(glm::ivec2 cell, T value) {

		std::uint32_t index = getIndex(cell);
		values[index] = value;
	}

	template<typename T>
	inline T Heightmap<T>::getValue(glm::ivec2 cell) const {

		std::uint32_t index = getIndex(cell);
		return values[index];
	}

	template<typename T>
	inline std::uint32_t Heightmap<T>::getIndex(glm::ivec2 cell) const {
		return cell.y * dimensions.x + cell.x;
	}

	template<typename T>
	inline glm::ivec2 Heightmap<T>::getSize() const {
		return dimensions;
	}

}