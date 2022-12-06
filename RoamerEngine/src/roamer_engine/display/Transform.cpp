#include "roamer_engine/display/Transform.hpp"
#include "roamer_engine/display/DisplayObject.hpp"
#include <vector>

namespace qy::cg {

	struct Transform::Impl {
		glm::vec3 position {0, 0, 0};
		glm::vec3 scale {1, 1, 1};
		glm::quat rotation {0, 0, 0, 0};

		TransformPtr parent;
		std::vector<TransformPtr> children;
	};

	DEFINE_OBJECT(Transform);

	glm::vec3 Transform::position() const { return pImpl->position; }
	void Transform::position(const glm::vec3& value) { pImpl->position = value; }

	glm::vec3 Transform::scale() const { return pImpl->scale; }
	void Transform::scale(const glm::vec3& value) { pImpl->scale = value; }

	glm::quat Transform::rotation() const { return pImpl->rotation; }
	void Transform::rotation(const glm::quat& value) { pImpl->rotation = value; }

	glm::mat4 Transform::modelMatrix() const {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pImpl->position);
		model = model * glm::mat4_cast(pImpl->rotation);
		model = glm::scale(model, pImpl->scale);
		return model;
	}

	// TODO 这个matrix没考虑父子关系
	glm::mat4x4 Transform::localToWorldMatrix() const {
		return modelMatrix();
	}

	void Transform::lookAt(const glm::vec3& target) {
		// TODO
	}

	TransformPtr Transform::parent() const { return pImpl->parent; }

	size_t Transform::childCount() const {
		return pImpl->children.size();
	}

	Transform::const_iterator Transform::begin() const { return pImpl->children.begin(); }

	Transform::const_iterator Transform::end() const { return pImpl->children.end(); }

	void Transform::addChild(pointer child) {
		pImpl->children.push_back(child);
		child->pImpl->parent = std::dynamic_pointer_cast<Transform>(shared_from_this());
	}

	void Transform::removeChild(pointer child) {
		std::erase(pImpl->children, child);
		child->pImpl->parent = nullptr;
	}

	Transform::const_iterator begin(const TransformPtr& pt) { return pt->begin(); }
	Transform::const_iterator end(const TransformPtr& pt) { return pt->end(); }

}