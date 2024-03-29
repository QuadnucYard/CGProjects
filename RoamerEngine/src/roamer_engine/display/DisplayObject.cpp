﻿#include "roamer_engine/display/DisplayObject.hpp"
#include "roamer_engine/display/Transform.hpp"

namespace qy::cg {

	struct DisplayObject::Impl {
		bool active {true};
		ptr<Transform> transform;
		ptr_vector<Component> components;
	};

	DEFINE_OBJECT(DisplayObject);

	void DisplayObject::_init() {
		pImpl->transform = addComponent<Transform>();
	}

	ptr<DisplayObject> DisplayObject::create() {
		return create("DisplayObject");
	}

	ptr<DisplayObject> DisplayObject::create(std::string name) {
		auto obj = instantiate<DisplayObject>();
		obj->name(std::move(name));
		obj->_init();
		return obj;
	}

	bool DisplayObject::activeSelf() const {
		return pImpl->active;
	}

	bool DisplayObject::activeInHierarchy() const {
		for (auto&& tr = pImpl->transform; tr; tr = tr->parent()) {
			if (!tr->obj()->activeSelf()) return false;
		}
		return true;
	}

	void DisplayObject::setActive(bool value = true) {
		pImpl->active = value;
	}

	TransformPtr DisplayObject::transform() const { return pImpl->transform; }

	ptr<Component> DisplayObject::addComponent(const ptr<Component>& component) {
		pImpl->components.push_back(component);
		component->_setObj(shared_from_this());
		component->start();
		return component;
	}

	const ptr_vector<Component>& DisplayObject::getComponents() const {
		return pImpl->components;
	}

	void DisplayObject::update() {
		for (auto&& c : getComponents()) {
			if (c->enabled())
				c->update();
		}
		for (auto&& c : transform()) {
			if (c->obj()->activeSelf())
				c->obj()->update();
		}
	}

}