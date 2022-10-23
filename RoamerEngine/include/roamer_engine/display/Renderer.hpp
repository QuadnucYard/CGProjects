#pragma once
#include "Component.hpp"
#include "Material.hpp"

namespace qy::cg {
	class Renderer: public Component {
		ptr_vector<Material> m_materials;
		ptr_vector<Material> m_sharedMaterials;
	public:
		DECL_OBJECT(Renderer);

		ptr<Material> getMaterial() { 
			if (m_materials.at(0) == m_sharedMaterials.at(0))
				m_materials[0] = m_materials[0]->clone();
			return  m_materials[0]; 
		}

		ptr_vector<Material> getMaterials() {
			for (size_t i = 0; i < m_materials.size(); i++) {
				if (m_materials[i] == m_sharedMaterials[i]) 
					m_materials[i] = m_materials[i]->clone();
			}
			return m_materials;
		}

		void setMaterial(const ptr<Material>& material) {
			if (m_materials.empty()) m_materials.resize(1);
			if (m_sharedMaterials.empty()) m_sharedMaterials.resize(1);
			m_materials.at(0) = m_sharedMaterials.at(0) = material; 
		}

		void setMaterials(const ptr_vector<Material>& materials) {
			m_materials = m_sharedMaterials = materials;
		}

		ptr<Material> getSharedMaterial() { 
			return m_sharedMaterials.at(0); 
		}

		ptr_vector<Material> getSharedMaterials() { 
			return m_sharedMaterials; 
		}
	};
}