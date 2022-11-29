#pragma once
#include "recfg.hpp"
#include <map>
#include <functional>
#include <nameof.hpp>

namespace qy::cg {

	namespace fs = std::filesystem;

	class Object {
	public:
		Object() = default;
		Object(const Object& other) = delete;

		const std::string& name() const { return m_name; }
		void name(std::string value) { m_name = std::move(value); }

	protected:
		std::string m_name;
	};


#define DECL_OBJECT(class_) \
	class_(); \
	~class_(); \
	RegisterType(class_)

#define DECL_PIMPL \
	struct Impl; \
	impl_ptr<Impl> pImpl;

#define MAKE_PIMPL \
	pImpl(std::make_unique<Impl>())

#define DEFINE_OBJECT(class_) \
	class_::class_() : pImpl(std::make_unique<class_::Impl>()) {} \
	class_::~class_() = default;

	////////////////////////////////////////////////////////////////////////////////
	// Get object by name

	struct ObjectFactory {
		inline static std::map<std::string, std::function<Object*()>> type_creator_map;

		static Object* create(const std::string& type)
		{
			if (auto it = type_creator_map.find(type); it != type_creator_map.end())
				return it->second();
			return nullptr;
		}

		template <typename T>
		static std::enable_if_t<std::is_base_of_v<Object, T>, std::unique_ptr<T>>
			create(const std::string& type)
		{
			return std::unique_ptr<T>(static_cast<T*>(create(type)));
		}

		template <typename T>
		struct RegisterTypeHelper {
			RegisterTypeHelper(const std::string& id) {
				if constexpr (std::is_constructible_v<T>) {
					type_creator_map.insert({id, []() { return new T(); }});
					type_creator_map.insert({std::string(NAMEOF_TYPE(T)), []() { return new T(); }});
				}
			}
		};
	};

#define RegisterType(Type) \
	inline static ObjectFactory::RegisterTypeHelper<Type> __register_type_global_##Type{#Type};

}