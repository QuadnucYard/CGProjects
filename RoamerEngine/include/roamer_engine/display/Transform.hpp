#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.hpp"
#include <vector>

namespace qy::cg {

	class Transform : public Component {
	public:
		using pointer = ptr<Transform>;
		using const_iterator = std::vector<pointer>::const_iterator;

		DECL_OBJECT(Transform);

		vec3 position() const;
		void position(const vec3& value);

		vec3 scale() const;
		void scale(const vec3& value);

		quat rotation() const;
		void rotation(const quat& value);

		vec3 worldPosition() const;
		void worldPosition(const vec3& value);

		quat worldRotation() const;
		void worldRotation(const quat& value);

		vec3 right() const;
		vec3 up() const;
		vec3 forward() const;

		mat4 modelMatrix() const;

		mat4 localToWorldMatrix() const;

		mat4 worldToLocalMatrix() const;

		void lookAt(const vec3& target);

		pointer parent() const;

		pointer root() const;

		size_t childCount() const;

		void addChild(pointer child);

		void removeChild(pointer child);

		void setParent(pointer parent);

		const_iterator begin() const;
		const_iterator end() const;

	private:
		DECL_PIMPL;

	};

	using TransformPtr = Transform::pointer;

	Transform::const_iterator begin(const TransformPtr& pt);
	Transform::const_iterator end(const TransformPtr& pt);
	
}