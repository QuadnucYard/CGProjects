#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace qy::cg {

	class Transform {

	private:
		glm::vec3 _position {0, 0, 0};
		glm::vec3 _scale {1, 1, 1};
		glm::quat _rotation {0, 0, 0, 0};

	public:
		glm::vec3 position() const { return _position; }
		glm::vec3& position() { return _position; }

		glm::vec3 scale() const { return _scale; }
		glm::vec3& scale() { return _scale; }

		glm::quat rotation() const { return _rotation; }
		glm::quat& rotation() { return _rotation; }

		// TODO 这个matrix没考虑父子关系
		glm::mat4x4 localToWorldMatrix() const {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, _scale);
			model = glm::mat4_cast(_rotation) * model;
			model = glm::translate(model, _position);
			return model;
		}

	};
}