#include "roamer_engine/display/Transform.hpp"
#include "roamer_engine/display/DisplayObject.hpp"
#include <vector>

namespace qy::cg {

	struct Transform::Impl {
		vec3 position {0, 0, 0};
		vec3 scale {1, 1, 1};
		quat rotation {1, 0, 0, 0}; // In ctor, args are (w, x, y, z)

		TransformPtr parent;
		std::vector<TransformPtr> children;
	};

	DEFINE_OBJECT(Transform);

	vec3 Transform::position() const { return pImpl->position; }
	void Transform::position(const vec3& value) { pImpl->position = value; }

	vec3 Transform::scale() const { return pImpl->scale; }
	void Transform::scale(const vec3& value) { pImpl->scale = value; }

	quat Transform::rotation() const { return pImpl->rotation; }
	void Transform::rotation(const quat& value) { pImpl->rotation = value; }

	vec3 Transform::worldPosition() const {
		if (!pImpl->parent) return pImpl->position;
		return pImpl->parent->localToWorldMatrix() * vec4(pImpl->position, 1.0f);
	}

	void Transform::worldPosition(const vec3& value) {
		if (!pImpl->parent) pImpl->position = value;
		else pImpl->position = pImpl->parent->worldToLocalMatrix() * vec4(value, 1.0f);
	}

	quat Transform::worldRotation() const {
		if (!pImpl->parent)	return pImpl->rotation;
		return pImpl->parent->worldRotation() * pImpl->rotation;
	}

	void Transform::worldRotation(const quat& value) { // TODO
		throw "Not implemented";
	}

	vec3 Transform::right() const { return pImpl->rotation * glm::vec3_right; }

	vec3 Transform::up() const { return pImpl->rotation * glm::vec3_up; }

	vec3 Transform::forward() const { return pImpl->rotation * glm::vec3_forward; }

	mat4 Transform::modelMatrix() const {
		mat4 model = mat4(1.0f);
		model = glm::translate(model, pImpl->position);
		model = model * glm::mat4_cast(pImpl->rotation);
		model = glm::scale(model, pImpl->scale);
		return model;
	}

	mat4 Transform::localToWorldMatrix() const {
		if (pImpl->parent) return pImpl->parent->localToWorldMatrix() * modelMatrix();
		return modelMatrix();
	}

	mat4 Transform::worldToLocalMatrix() const {
		return glm::inverse(localToWorldMatrix());
	}

	void Transform::lookAt(const vec3& target) {
		glm::quatLookAt(target - worldPosition(), up());
	}

	TransformPtr Transform::parent() const { return pImpl->parent; }

	TransformPtr Transform::root() const {
		if (pImpl->parent) return pImpl->parent->root();
		return std::dynamic_pointer_cast<Transform>(std::const_pointer_cast<Component>(shared_from_this()));
	}

	size_t Transform::childCount() const {
		return pImpl->children.size();
	}

	Transform::const_iterator Transform::begin() const { return pImpl->children.begin(); }

	Transform::const_iterator Transform::end() const { return pImpl->children.end(); }

	void Transform::addChild(pointer child) {
		if (!child->pImpl->parent) {
			child->pImpl->parent = std::dynamic_pointer_cast<Transform>(shared_from_this());
			pImpl->children.push_back(std::move(child));
		}
	}

	void Transform::removeChild(pointer child) {
		if (child->pImpl->parent.get() == this) {
			std::erase(pImpl->children, child);
			child->pImpl->parent = nullptr;
		}
	}

	void Transform::setParent(pointer parent) {
		if (pImpl->parent)
			std::erase_if(pImpl->parent->pImpl->children, [this](auto&& t) { return t.get() == this; });
		pImpl->parent = parent;
		if (parent)
			pImpl->parent->pImpl->children.emplace_back(this);
	}

	Transform::const_iterator begin(const TransformPtr& pt) { return pt->begin(); }
	Transform::const_iterator end(const TransformPtr& pt) { return pt->end(); }

}