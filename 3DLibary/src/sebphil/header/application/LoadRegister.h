#pragma once
#include <memory>
#include <unordered_map>

namespace glib {

	template <class Key, class Value>
	class LoadRegister {

	public:
		template <class ...Args>
		std::shared_ptr<Value> load(const Key& key, Args&& ...args);

		std::size_t getSize() const;

	private:
		// register is already taken by a keyword
		std::unordered_map<Key, std::weak_ptr<Value>> registry;

		template <class ...Args>
		std::shared_ptr<Value> makeNewEntry(const Key& key, Args&& ...args);
		
		template <class ...Args>
		std::shared_ptr<Value> manageWeakptr(const Key& key, Args&& ...args);

	};

	template<class Key, class Value>
	template<class ...Args>
	inline std::shared_ptr<Value> glib::LoadRegister<Key, Value>::load(const Key& key, Args&& ...args) {

		auto itr = registry.find(key);
		std::shared_ptr<Value> ptr;

		if(itr == registry.end())
			ptr = makeNewEntry(key, std::forward<Args>(args)...);
		else
			ptr = manageWeakptr(key, std::forward<Args>(args)...);

		return ptr;
	}

	template<class Key, class Value>
	template<class ...Args>
	inline std::shared_ptr<Value> LoadRegister<Key, Value>::manageWeakptr(const Key& key, Args && ...args) {

		std::shared_ptr<Value> ptr;
		const std::weak_ptr<Value>& weakPtr = registry[key];

		if (weakPtr.expired())
			ptr = makeNewEntry(key, std::forward<Args>(args)...);
		else
			ptr = std::shared_ptr<Value>(weakPtr);

		return ptr;
	}

	template<class Key, class Value>
	template<class ...Args>
	inline std::shared_ptr<Value> LoadRegister<Key, Value>::makeNewEntry(const Key& key, Args && ...args) {

		std::shared_ptr<Value> ptr = std::make_shared<Value>(std::forward<Args>(args)...);
		registry[key] = ptr;

		return ptr;
	}

	template<class Key, class Value>
	inline std::size_t LoadRegister<Key, Value>::getSize() const {
		return registry.size();
	}

}