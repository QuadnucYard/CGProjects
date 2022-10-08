#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.h"
#include <vector>

namespace qy::cg {

	class Transform : public Component {
	public:
		using pointer = std::shared_ptr<Transform>;

		DECL_OBJECT(Transform)
		//Transform();
		//~Transform();

		glm::vec3 position() const;
		glm::vec3& position();

		glm::vec3 scale() const;
		glm::vec3& scale();

		glm::quat rotation() const;
		glm::quat& rotation();

		glm::mat4x4 localToWorldMatrix() const;

		pointer parent() const;

		void add_child(pointer child);

		void remove_child(pointer child);

		std::vector<std::shared_ptr<Transform>>::const_iterator begin() const;
		std::vector<std::shared_ptr<Transform>>::const_iterator end() const;

	private:
		struct Impl;
		impl_ptr<Impl> pImpl;

	};

	using TransformPtr = Transform::pointer;

	//auto begin(const TransformPtr& pt) { return pt->begin(); }
	//auto end(const TransformPtr& pt) { return pt->end(); }
	
}