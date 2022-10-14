#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.hpp"
#include <vector>

namespace qy::cg {

	class Transform : public Component {
	public:
		using pointer = std::shared_ptr<Transform>;
		using const_iterator = std::vector<pointer>::const_iterator;

		DECL_OBJECT(Transform);

		glm::vec3 position() const;
		void position(const glm::vec3& value);

		glm::vec3 scale() const;
		void scale(const glm::vec3& value);

		glm::quat rotation() const;
		void rotation(const glm::quat& value);

		glm::mat4x4 localToWorldMatrix() const;

		pointer parent() const;

		void addChild(pointer child);

		void removeChild(pointer child);

		const_iterator begin() const;
		const_iterator end() const;

	private:
		DECL_PIMPL;

	};

	using TransformPtr = Transform::pointer;

	Transform::const_iterator begin(const TransformPtr& pt);
	Transform::const_iterator end(const TransformPtr& pt);
	
}