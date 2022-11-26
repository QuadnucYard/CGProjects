#pragma once
#include "recfg.hpp"

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
	~class_();

#define DECL_PIMPL \
	struct Impl; \
	impl_ptr<Impl> pImpl;

#define MAKE_PIMPL \
	pImpl(std::make_unique<Impl>())

#define DEFINE_OBJECT(class_) \
	class_::class_() : pImpl(std::make_unique<class_::Impl>()) {} \
	class_::~class_() = default;

}