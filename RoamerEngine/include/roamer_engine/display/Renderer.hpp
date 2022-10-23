#pragma once
#include "Component.hpp"
#include "Material.hpp"

namespace qy::cg {
	class Renderer: public Component {
		ptr<Material> m_material;
	public:
		DECL_OBJECT(Renderer);

		ptr<Material> getMaterial() { return m_material; }
		void setMaterial(const ptr<Material>& material) { m_material = material; }

		virtual void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) = 0;
	};
}