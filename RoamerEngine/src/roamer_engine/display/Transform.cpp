#include "roamer_engine/display/Transform.h"
#include "roamer_engine/display/DisplayObject.h"
#include <vector>

namespace qy::cg {

	struct Transform::Impl {
		glm::vec3 _position {0, 0, 0};
		glm::vec3 _scale {1, 1, 1};
		glm::quat _rotation {0, 0, 0, 0};

		TransformPtr m_parent;
		std::vector<TransformPtr> m_children;
	};

	DEFINE_OBJECT(Transform)

	glm::vec3 Transform::position() const { return pImpl->_position; }
	glm::vec3& Transform::position() { return pImpl->_position; }
	glm::vec3 Transform::scale() const { return pImpl->_scale; }
	glm::vec3& Transform::scale() { return pImpl->_scale; }
	glm::quat Transform::rotation() const { return pImpl->_rotation; }
	glm::quat& Transform::rotation() { return  pImpl->_rotation; }

	// TODO 这个matrix没考虑父子关系
	glm::mat4x4 Transform::localToWorldMatrix() const {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, pImpl->_scale);
		model = glm::mat4_cast(pImpl->_rotation) * model;
		model = glm::translate(model, pImpl->_position);
		return model;
	}
	TransformPtr Transform::parent() const { return pImpl->m_parent; }

	std::vector<std::shared_ptr<Transform>>::const_iterator Transform::begin() const { return pImpl->m_children.begin(); }

	std::vector<std::shared_ptr<Transform>>::const_iterator Transform::end() const { return pImpl->m_children.end(); }

	void Transform::add_child(pointer child) {
		pImpl->m_children.push_back(child);
		//auto x = obj();
		//auto y = obj()->transform();
		child->pImpl->m_parent = std::dynamic_pointer_cast<Transform>(shared_from_this());
	}

	void Transform::remove_child(pointer child) {
		std::erase(pImpl->m_children, child);
		child->pImpl->m_parent = nullptr;
	}

}